/*
 * GraphView.h
 * View to display graph, including path of bees
 *
 * $HeadURL$
 * $Id$
 */

#import <Cocoa/Cocoa.h>

@class Graph;

@interface GraphView : NSView {
	double scaleX, scaleY;
	IBOutlet Graph *graph;
}

- (void)updateTransform;
- (void)windowDidResize:(NSNotification *)notification;
- (IBAction)displayChange:(id) sender;
- (void) saveToPDF: (NSString *) fileName;

@end
