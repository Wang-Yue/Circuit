//
//  ChannelRunner.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef ChannelRunner_hpp
#define ChannelRunner_hpp

#include "TypeDefs.hpp"

#include <list>
#include <set>
#include <vector>

#include "Sample.hpp"
#include "Synth.hpp"
#include "Step.hpp"
#include "PatternChainRunner.hpp"
#include "Channel.hpp"

#include "ChannelOutputFactory.hpp"
#include "ChannelOutputInterface.hpp"
#include "Session.hpp"

template <typename AtomClass> class Channel;
template <typename AtomClass> class PatternChainRunner;

template <typename AtomClass>
class BaseChannelRunner {
public:
  BaseChannelRunner(Channel<AtomClass> *channel)  : _channel(channel), _microstep_tick_counter(0) {
    _pattern_chain_runner = new PatternChainRunner<AtomClass>(channel->GetPatternChain());
  }
  virtual ~BaseChannelRunner() {
    delete _pattern_chain_runner;
  }

  virtual void Stop() {
  }
  
  virtual void Restart() {
    // It's a good practice to stop before the restart --- Child class may do extra work in stop
    // like clearing up the playing notes.
    Stop();
    _microstep_tick_counter = 0;
    GetPatternChainRunner()->Restart();
  }

  // NotNull
  PatternChainRunner<AtomClass> *GetPatternChainRunner() const  {
    return _pattern_chain_runner;
  }
  Microstep GetMicrostepTickCounter() const  {
    return _microstep_tick_counter;
  }

  virtual void TickMicrostep()  {
    if (_microstep_tick_counter < kMicrosteps - 1) {
      ++_microstep_tick_counter;
      return;
    }
    _microstep_tick_counter = 0;
  }

  CC GetDefaultCC(const Control &control) const {
    return _channel->GetDefaultCC(control);
  }
  
  void SetDefaultCC(const Control &control, const CC &cc) {
    return _channel->SetDefaultCC(control, cc);
  }
  
protected:
  Channel<AtomClass> *_channel;
  PatternChainRunner<AtomClass> *_pattern_chain_runner;
  Microstep _microstep_tick_counter;
};

template <typename AtomClass>
class ChannelRunner : public BaseChannelRunner<AtomClass> {
};



template <>
class ChannelRunner<Sample> : public BaseChannelRunner<Sample> {
public:
  ChannelRunner(Channel<Sample> *channel)  :
  BaseChannelRunner<Sample>(channel) {
    _output = ChannelOutputFactory::GetInstance().GetSampleChannelOutput(channel->GetChannelIndex());
  }

  virtual ~ChannelRunner()  {
  }

  virtual void TickMicrostep() override {
    if (GetMicrostepTickCounter() == 0) {
      Step<Sample> *step = GetPatternChainRunner()->GetStep();
      std::vector<Sample *> atoms = step->GetAtoms();
      assert(atoms.size() <= kSamplePolyphonyCapacity);
      for (Sample *atom : atoms) {
        ScheduleAtom(atom, step->GetMicrostepTicks());
      }
      // handle cc
      for (Control i = 0; i < kControlCapacity; ++i) {
        if (step->HasAutomation(i)) {
          CC cc = step->GetAutomation(i);
          _channel->SetDefaultCC(i, cc);
          _output->ControlChange(i, cc);
        }
      }
      GetPatternChainRunner()->TickStep();
    }
    PerformSchedule();
    BaseChannelRunner::TickMicrostep();
  }
  
private:
  void ScheduleAtom(const Sample *atom, const Microstep &microstep_ticks)  {
    for (Microstep i = 0; i < kMicrosteps; i ++) {
      SchedulerEvent event = {
        .tick = i,
        .sample_index = atom->IsSampleFlip() ? atom->GetSampleIndex() : _channel->GetSampleIndex(),
        .velocity = atom->GetVelocity(),
      };
      if (microstep_ticks & (1 << i)) {
        _scheduled_event.push_back(event);
      }
    }
  }
  void PerformSchedule() {
    for (std::list<SchedulerEvent>::iterator iter = _scheduled_event.begin();
         iter != _scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent &event = *iter;
      if (event.tick == 0) {
        _output->Play(event.velocity, event.sample_index);
        iter = _scheduled_event.erase(iter);
      } else {
        --event.tick;
        ++iter;
      }
    }
  }
  struct SchedulerEvent {
    Microstep tick;
    const Velocity velocity;
    const SampleIndex sample_index;
  };
  std::list<SchedulerEvent> _scheduled_event;
  SampleChannelOutputInterface *_output;
};


template <>
class ChannelRunner<Synth> : public BaseChannelRunner<Synth> {
public:
  ChannelRunner(Channel<Synth> *channel) :
  BaseChannelRunner<Synth>(channel) {
    _output = ChannelOutputFactory::GetInstance().GetSynthChannelOutput(channel->GetChannelIndex());
  }

  virtual ~ChannelRunner()  {
    Restart();
  }
  
  virtual void Stop() override {
    for (SchedulerEvent &event : _stop_scheduled_event) {
      _output->NoteOff(event.midi_note);
    }
    _stop_scheduled_event.clear();
  }
  
  virtual void TickMicrostep() override  {
    if (GetMicrostepTickCounter() == 0) {
      Step<Synth> *step = GetPatternChainRunner()->GetStep();
      std::vector<Synth *> atoms = step->GetAtoms();
      assert(atoms.size() <= kSynthPolyphonyCapacity);
      for (Synth *atom : atoms) {
        SchedulePlay(atom, step->GetMicrostepDelay());
      }
      // handle cc.
      for (Control i = 0; i < kControlCapacity; ++i) {
        if (step->HasAutomation(i)) {
          CC cc = step->GetAutomation(i);
          _channel->SetDefaultCC(i, cc);
          _output->ControlChange(i, cc);
        }
      }
    }
    if (GetMicrostepTickCounter() == kMicrosteps - 1) {
      GetPatternChainRunner()->TickStep();
    }
    PerformSchedule();
    BaseChannelRunner::TickMicrostep();
  }
  

  
  std::list<Note> GetPlayingNotes() const {
    std::list<Note> notes;
    for (SchedulerEvent event : _stop_scheduled_event) {
      notes.push_back(event.note);
    }
    return notes;
  }
  
private:
  void SchedulePlay(const Synth *atom, const Microstep &microstep_delay) {
    assert(microstep_delay < kMicrosteps);
    // TODO : Is this correct? what if the current session is changed?
    Session *session = _channel->GetSession();
    Note converted_note = ConvertNoteScale(atom->GetNote(), session->GetBaseNote().scale);
    MIDINote midi_note = NoteToMIDI(converted_note, session->GetTonicDegree());
    SchedulerEvent event = {
      .tick = microstep_delay,
      .midi_note = midi_note,
      .velocity = atom->GetVelocity(),
      .note = atom->GetNote(),
      .gate = atom->GetGate(),
      .tie = atom->GetTie(),
    };
    _play_scheduled_event.push_back(event);
  }

  void PerformSchedule()  {
    // Instead of stopping the note, put tie note in this set, and don't replay them.
    std::set<MIDINote> tie_notes;
    for (std::list<SchedulerEvent>::iterator iter = _stop_scheduled_event.begin();
         iter != _stop_scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent &event = *iter;

      if (event.tick == 0) {
        if (event.tie) {
          tie_notes.insert(event.midi_note);
        } else {
          _output->NoteOff(event.midi_note);
        }
        iter = _stop_scheduled_event.erase(iter);
      } else {
        --event.tick;
        ++iter;
      }
    }
    for (std::list<SchedulerEvent>::iterator iter = _play_scheduled_event.begin();
         iter != _play_scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent &event = *iter;
      if (event.tick == 0) {
        if (event.tie && tie_notes.count(event.midi_note)) {
          tie_notes.erase(event.midi_note);
        } else {
          _output->NoteOn(event.midi_note, event.velocity);
        }
        SchedulerEvent stop_event = {
          .tick = event.gate,
          .midi_note = event.midi_note,
          .velocity = event.velocity,
          .note = event.note,
          .gate = event.gate,
          .tie = event.tie,
        };
        _stop_scheduled_event.push_back(stop_event);
        iter = _play_scheduled_event.erase(iter);
        
      } else {
        --event.tick;
        ++iter;
      }
    }
    for (const MIDINote &note : tie_notes) {
      _output->NoteOff(note);
    }
  }
  struct SchedulerEvent {
    Microstep tick;
    const MIDINote midi_note;
    const Velocity velocity;
    const Note note;
    const Gate gate;
    const bool tie;
  };
  
  std::list<SchedulerEvent> _play_scheduled_event;
  std::list<SchedulerEvent> _stop_scheduled_event;
  SynthChannelOutputInterface *_output;
};

#endif /* ChannelRunner_hpp */
