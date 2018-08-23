//
//  ViewController.m
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#import "ViewController.h"

#import "Bridge.hpp"

// Quick and hacky way to make the test environment working.

void Beep() {
  dispatch_async(dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void){
    usleep(1000*100);
    NSBeep();
  });

}

@class Tapview;

@interface MyManager : NSObject
@property (nonatomic, retain) NSMutableArray *keys;
@property (nonatomic, retain) NSMutableArray *keyEvents;
@property (nonatomic, retain) NSMutableArray *views;
+ (instancetype)sharedManager;
- (void)key:(NSUInteger)key event:(BOOL)up;
@end


@implementation MyManager

+ (instancetype)sharedManager {
  static MyManager *sharedMyManager = nil;
  @synchronized(self) {
    if (sharedMyManager == nil)
      sharedMyManager = [[self alloc] init];
  }
  return sharedMyManager;
}

- (instancetype)init {
  if (self = [super init]) {
    _keys = [NSMutableArray new];
    _keyEvents = [NSMutableArray new];
    _views = [NSMutableArray new];
  }
  return self;
}

- (void)key:(NSUInteger)key event:(BOOL)event {
  [_keys addObject:@(key)];
  [_keyEvents addObject:@(event)];
}
@end


@interface TapView : NSView
@property (nonatomic) char uid;
@property (nonatomic) uint32_t colorCode;
@end

@implementation TapView
- (instancetype)initWithFrame:(NSRect)frameRect uid:(char)uid {
  self = [self initWithFrame:frameRect];
  if (self) {
    _uid = uid;
  }
  return self;
}

- (void)setColorCode:(uint32_t)colorCode {
  if (colorCode == _colorCode) {
    return;
  }
  _colorCode = colorCode;
  
  NSColor *color = [NSColor colorWithRed:(colorCode & 0xff) / 256.0
                                   green:((colorCode >> 8) & 0xff) / 256.0
                                    blue:((colorCode >> 16) & 0xff) / 256.0
                                   alpha:1];
  [self.layer setBackgroundColor:[color CGColor]];
}

- (void)mouseDown:(NSEvent *)event {
  [[MyManager sharedManager] key:_uid event:YES];
}
- (void)mouseUp:(NSEvent *)event {
  [[MyManager sharedManager] key:_uid event:NO];
}

@end



@interface CircuitView : NSView
@end

@implementation CircuitView

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self) {
    NSUInteger uid = 0;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 8; ++j) {
        CGFloat x = j * 50;
        CGFloat y = i * 50;
        NSView *view = [[TapView alloc] initWithFrame: NSMakeRect( x, y, 45.0, 45.0 ) uid:uid];
        [view setWantsLayer:YES];
        NSColor *color = [NSColor blackColor] ;
        [view.layer setBackgroundColor:[color CGColor]];
        [view becomeFirstResponder];
        [[MyManager sharedManager].views addObject:view];
        [self addSubview:view];
        ++uid;
      }
    }
  }
  InitializeCircuit();
  return self;
}

- (BOOL)isFlipped {
  return YES;
}
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  CircuitView *view = [[CircuitView alloc] initWithFrame:self.view.frame];
  [self.view addSubview:view];
  [NSTimer scheduledTimerWithTimeInterval:0.1f
                                   target:self
                                 selector:@selector(loopCircuit)
                                 userInfo:nil
                                  repeats:YES];

  // Do any additional setup after loading the view.
}

- (void)loopCircuit {
  NSMutableArray *keys = [MyManager sharedManager].keys;
  NSMutableArray *events = [MyManager sharedManager].keyEvents;
  NSMutableArray *views = [MyManager sharedManager].views;

  for (int i = 0; i < keys.count; ++i) {
    NSNumber *key = keys[i];
    NSNumber *event = events[i];
    char k = [key charValue];
    BOOL e = [event boolValue];
    if (e) {
      PressKey(k);
    } else {
      ReleaseKey(k);
    }
  }
  [keys removeAllObjects];
  [events removeAllObjects];
  for (int i = 0; i < views.count; ++i) {
    TapView *view = views[i];
    uint32_t c = GetColor(i);
    [view setColorCode:c];
  }
  Tick();
  ++_tick;
  _tick%=6;
}

- (void)setRepresentedObject:(id)representedObject {
  [super setRepresentedObject:representedObject];

  // Update the view, if already loaded.
}


@end
