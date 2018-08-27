//
//  ViewController.m
//  CircuitApp
//
//  Created by Yue Wang on 8/19/18.
//  Copyright © 2018 Yue Wang. All rights reserved.
//

#import "ViewController.h"

#import "Bridge.h"

// Quick and hacky way to make the test environment working.


@interface CircuitView : NSView
- (void)setColorCode:(uint32_t)colorCode forPad:(size_t)pad;
@end

static CircuitView *_circuitView = NULL;

void WritePadColor(const uint32_t pad, const uint32_t color) {
  if (!_circuitView) {
    return;
  }
    dispatch_async(dispatch_get_main_queue(), ^(void){
      [_circuitView setColorCode:color forPad:pad];
    });
}

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
  TapPad(_uid);
}

- (void)mouseUp:(NSEvent *)event {
  ReleasePad(_uid);
}

@end

@implementation CircuitView {
  NSMutableArray *_viewArray;
}

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self) {
    _viewArray = [NSMutableArray array];
    NSUInteger uid = 0;
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 8; ++j) {
        CGFloat x = j * 50;
        CGFloat y = i * 50;
        NSView *view = [[TapView alloc] initWithFrame: NSMakeRect( x, y, 45.0, 45.0 ) uid:uid];
        [view setWantsLayer:YES];
        NSColor *color = [NSColor blackColor] ;
        [view.layer setBackgroundColor:[color CGColor]];
        [view becomeFirstResponder];
        [self addSubview:view];
        [_viewArray addObject:view];
        ++uid;
      }
    }
  }
  InitializeCircuit();
  return self;
}

- (void)setColorCode:(uint32_t)colorCode forPad:(size_t)pad{
  if (pad >= _viewArray.count) {
    return;
  }
  TapView *view = _viewArray[pad];
  [view setColorCode:colorCode];
}

- (BOOL)isFlipped {
  return YES;
}
@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  _circuitView = [[CircuitView alloc] initWithFrame:self.view.frame];
  [self.view addSubview:_circuitView];
  // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
  [super setRepresentedObject:representedObject];
}


@end
