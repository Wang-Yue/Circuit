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


class MidiControllerDelegate {
public:
  virtual void NoteOn(const unsigned char &midi_note, const unsigned char &velocity) = 0;
  virtual void NoteOff(const unsigned char &midi_note) = 0;
};

class MidiController {
public:
  static MidiController& GetInstance() {
    static MidiController instance;
    return instance;
  }
  void SendPlayNoteMessage(const char &channel, const char &note, const char &velocity){
    std::vector<unsigned char> message;
    message.push_back(144 + channel);
    message.push_back(note);
    message.push_back(velocity);
    printf("Play note %d\n", note);
    _midi_out->sendMessage( &message );
  };
  void SendStopNoteMessage(const char &channel, const char &note){
    std::vector<unsigned char> message;
    message.push_back( 128 + channel);
    message.push_back(note);
    message.push_back(0);
    printf("Stop note %d\n", note);
    _midi_out->sendMessage( &message );
  };
  
  void SendProgramChange(const char &channel, const char &program){
    std::vector<unsigned char> message;
    message.push_back( 192 + channel);
    message.push_back(program);
    printf("Stop note %d\n", program);
    _midi_out->sendMessage( &message );
  };
  
  void SendControlMessage(const char &channel, const char &control, const char &cc){
    std::vector<unsigned char> message;
    message.push_back(176 + channel);
    message.push_back(control);
    message.push_back(cc);
    printf("Control note %d, %d\n", control, cc);
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
      if (_delegate) {
        _delegate->NoteOn(message[1], message[2]);
        return true;
      }
    }
    if (nBytes && message[0] == 128) {
      if (_delegate) {
        _delegate->NoteOff(message[1]);
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
