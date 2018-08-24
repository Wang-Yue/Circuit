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

#include "MidiController.hpp"
#include "Bridge.hpp"
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

  virtual void Restart() {
    _microstep_tick_counter = 0;
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
  }

  virtual ~ChannelRunner()  {
    Restart();
  }
  
  virtual void TickMicrostep() override {
    BaseChannelRunner::TickMicrostep();
    if (GetMicrostepTickCounter() == 0) {
      GetPatternChainRunner()->TickStep();
      Step<Sample> *step = GetPatternChainRunner()->GetStep();
      std::vector<Sample *> atoms = step->GetAtoms();
      assert(atoms.size() <= kSamplePolyphonyCapacity);
      for (Sample *atom : atoms) {
        ScheduleAtom(atom, step->GetMicrostepTicks());
      }
    }
    PerformSchedule();
  }

  virtual void Restart() override {
    BaseChannelRunner::Restart();
    GetPatternChainRunner()->Restart();
    _scheduled_event.clear();
  }
  
private:
  void SendStartCommand(const Sample *atom)  {
//    std::cout << GetTickCounter() << " play sample: " << atom << std::endl;
    Beep();
  }

  void SendStopCommand(const Sample *atom)  {
    // no-op.
  }

  void ScheduleAtom(const Sample *atom, const Microstep &microstep_ticks)  {
    for (Microstep i = 0; i < kMicrosteps; i ++) {
      if (microstep_ticks & (1 << i)) {
        SchedulerEvent *event = new SchedulerEvent({
          .tick = i,
          .atom = atom,
        });
        _scheduled_event.push_back(event);
      }
    }
  }
  void PerformSchedule() {
    for (std::list<SchedulerEvent *>::iterator iter = _scheduled_event.begin();
         iter != _scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent *event = *iter;
      if (event->tick == 0) {
        SendStartCommand(event->atom);
        SendStopCommand(event->atom);
        delete event;
        iter = _scheduled_event.erase(iter);
      } else {
        --event->tick;
        ++iter;
      }
    }
  }
  struct SchedulerEvent {
    Microstep tick;
    const Sample *atom;
  };
  std::list<SchedulerEvent *> _scheduled_event;
};


template <>
class ChannelRunner<Synth> : public BaseChannelRunner<Synth> {
public:
  ChannelRunner(Channel<Synth> *channel) : BaseChannelRunner<Synth>(channel){
  }

  virtual ~ChannelRunner()  {
    Restart();
  }
  
  virtual void TickMicrostep() override  {
    BaseChannelRunner::TickMicrostep();
    if (GetMicrostepTickCounter() == 0) {
      GetPatternChainRunner()->TickStep();
      Step<Synth> *step = GetPatternChainRunner()->GetStep();
      std::vector<Synth *> atoms = step->GetAtoms();
      assert(atoms.size() <= kSynthPolyphonyCapacity);
      for (Synth *atom : atoms) {
        SchedulePlayAtom(atom, step->GetMicrostepDelay());
      }
    }
    PerformSchedule();
  }
  
  virtual void Restart() override  {
    BaseChannelRunner::Restart();
    GetPatternChainRunner()->Restart();
    for (SchedulerEvent *event : _play_scheduled_event) {
      delete event;
    }
    _play_scheduled_event.clear();
    for (SchedulerEvent *event : _stop_scheduled_event) {
      SendStopCommand(event->atom);
      delete event;
    }
    _stop_scheduled_event.clear();
  }
  
  std::list<Note> GetPlayingNotes() const {
    std::list<Note> notes;
    for (SchedulerEvent *event : _stop_scheduled_event) {
      notes.push_back(event->atom->GetNote());
    }
    return notes;
  }
  
private:
  void SendStartCommand(const Synth *atom) {
    Note note = atom->GetNote();
    // TODO : Is this correct? what if the current session is changed?
    Session *session = _channel->GetSession();
    Note converted_note = ConvertNoteScale(note, session->GetBaseNote().scale);
    MIDINote midi = NoteToMIDI(converted_note, session->GetTonicDegree());
    MidiController::getInstance().SendPlayNoteMessage(midi, atom->GetVelocity());
  }

  void SendStopCommand(const Synth *atom) {
    Note note = atom->GetNote();
    Session *session = _channel->GetSession();
    Note converted_note = ConvertNoteScale(note, session->GetBaseNote().scale);
    MIDINote midi = NoteToMIDI(converted_note, session->GetTonicDegree());
    MidiController::getInstance().SendStopNoteMessage(midi);
  }
  void SchedulePlayAtom(const Synth *atom, const Microstep &microstep_delay) {
    assert(microstep_delay < kMicrosteps);
    SchedulerEvent *event = new SchedulerEvent({
      .tick = microstep_delay,
      .atom = atom,
    });
    _play_scheduled_event.push_back(event);
  }
  void ScheduleStopAtom(const Synth *atom)  {
    SchedulerEvent *event = new SchedulerEvent({
      .tick = atom->GetGate(),
      .atom = atom,
    });
    _stop_scheduled_event.push_back(event);
  }
  void PerformSchedule()  {
    for (std::list<SchedulerEvent *>::iterator iter = _play_scheduled_event.begin();
         iter != _play_scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent *event = *iter;
      if (event->tick == 0) {
        SendStartCommand(event->atom);
        ScheduleStopAtom(event->atom);
        delete event;
        iter = _play_scheduled_event.erase(iter);
      } else {
        --event->tick;
        ++iter;
      }
    }
    for (std::list<SchedulerEvent *>::iterator iter = _stop_scheduled_event.begin();
         iter != _stop_scheduled_event.end();
         /*nothing*/) {
      SchedulerEvent *event = *iter;
      if (event->tick == 0) {
        SendStopCommand(event->atom);
        delete event;
        iter = _stop_scheduled_event.erase(iter);
      } else {
        --event->tick;
        ++iter;
      }
    }
  }
  struct SchedulerEvent {
    Microstep tick;
    const Synth *atom;
  };
  
  std::list<SchedulerEvent *> _play_scheduled_event;
  std::list<SchedulerEvent *> _stop_scheduled_event;
};

#endif /* ChannelRunner_hpp */
