//
//  MidiController.hpp
//  Circuit
//
//  Created by Yue Wang on 8/18/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#ifndef MidiController_hpp
#define MidiController_hpp

#include <iostream>

#include <RtMidi.h>

#include "Synth.hpp"
#include "Scale.hpp"

class MidiControllerDelegate {
public:
  virtual void TapNote(const Note &note) = 0;
  virtual void ReleaseNote(const Note &note) = 0;
};

class MidiController {
public:
  static MidiController& getInstance()
  {
    static MidiController instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
  }
  void SendPlayNoteMessage(const MIDINote &note, const Velocity &velocity){
    std::vector<unsigned char> message;
    message.push_back( 144 );
    message.push_back(note);
    message.push_back(velocity);
    std::cout << "Play note" << (int)message[1] << std::endl;
    _midi_out->sendMessage( &message );
  };
  void SendStopNoteMessage(const MIDINote &note){
    std::vector<unsigned char> message;
    message.push_back( 128 );
    message.push_back(note);
    message.push_back(0);
    std::cout << "Stop note" << (int)message[1] << std::endl;
    _midi_out->sendMessage( &message );
  };
  
  void Tick() {
    while (ReceiveNoteMessage()) {
    }
  }
  
  const bool ReceiveNoteMessage(){
    std::vector<unsigned char> message;
    _midi_in->ignoreTypes( true, true, true );
    _midi_in->getMessage( &message );
    size_t nBytes = message.size();
    if (nBytes && message[0] == 144) {
      Note note = MIDIToNote(message[1], ScaleChromatic, 0);
      std::cout << static_cast<int>(note.octave) << " " << static_cast<int>(note.degree) << std::endl;
      if (_delegate) {
        _delegate->TapNote(note);
        return true;
      }
    }
    if (nBytes && message[0] == 128) {
      Note note = MIDIToNote(message[1], ScaleChromatic, 0);
      if (_delegate) {
        _delegate->ReleaseNote(note);
        return true;
      }
    }
    return false;
  }
private:
  MidiController() {

    _midi_in = 0;
    _midi_out = 0;
    // RtMidiIn constructor
    try {
      _midi_in = new RtMidiIn();
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }
    // Check inputs.
    unsigned int nPorts = _midi_in->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try {
        portName = _midi_in->getPortName(i);
        if (portName.find("MPK")!=std::string::npos) {
          _midi_in->openPort(i);
        }
      }
      catch ( RtMidiError &error ) {
        error.printMessage();
        delete _midi_in;
      }
      std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
    }
    // RtMidiOut constructor
    try {
      _midi_out = new RtMidiOut();
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }
    // Check outputs.
    nPorts = _midi_out->getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try {
        portName = _midi_out->getPortName(i);
        if (portName.find("IAC")!=std::string::npos) {
          _midi_out->openPort(i);
        }
      }
      catch (RtMidiError &error) {
        error.printMessage();
        delete _midi_out;
      }
      std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
    }


  }
  RtMidiIn  *_midi_in = 0;
  RtMidiOut *_midi_out = 0;
  MidiControllerDelegate *_delegate = nullptr;
public:
  MidiController(MidiController const&)               = delete;
  void operator=(MidiController const&)  = delete;
  void SetDelegate(MidiControllerDelegate *delegate) {
    _delegate = delegate;
  }
};

#endif /* MidiController_hpp */
