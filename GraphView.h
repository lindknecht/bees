/*
 * GraphView.h
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL: svn://localhost/nadia/diplomarbeit/software/GraphView.h $
 * $Id: GraphView.h 183 2008-07-27 11:33:18Z kthul $
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
