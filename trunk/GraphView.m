/*
 * GraphView.m
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL: svn://localhost/nadia/diplomarbeit/software/GraphView.m $
 * $Id: GraphView.m 220 2008-08-17 09:48:14Z kthul $
 */

#import <math.h>
#import "GraphView.h"
#import "Graph.h"
#import "bienen.h"
#import "globals.h"

@implementation GraphView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    return self;
}

- (NSPoint) positionKnoten: (int) i {
	double x = Knoten[i].x;
	double y = Knoten[i].y;		
	return NSMakePoint(x, y);
}

- (void)drawRect:(NSRect)rect {
	if ([[graph showLinkStrength] boolValue]) {
		for (int i = 0; i < numKnoten; i++) {
			for (int j = 0; j < numKnoten; j++) {
				NSBezierPath *bp = [[NSBezierPath alloc] init];
				[bp setLineWidth:scaleX*0.5];
				[[NSColor colorWithDeviceWhite: 1 - n_sum[i][j]/n_sum_max alpha:0.8] setStroke];
				[bp moveToPoint: [self positionKnoten: i]];
				[bp lineToPoint: [self positionKnoten: j]];
				[bp stroke];
				[bp release];
			}
		}
	}	
	if ([[graph showAllBees] boolValue]) {
		for (int i = 0; i < numBienen; i++) {
			NSBezierPath *bp = [[NSBezierPath alloc] init];
			[bp setLineWidth:scaleX*0.5];
			[[NSColor colorWithCalibratedHue:1 - ((double) i) / numBienen saturation:1 brightness:1 alpha:0.9] setStroke]; 
//			[[NSColor colorWithDeviceWhite: 0.5 alpha:0.5] setStroke];
			[bp moveToPoint: [self positionKnoten: bienen[i].path[0]]];
			for	(int j = 1; j <= bienen[i].steps_path; j++)
				[bp lineToPoint: [self positionKnoten: bienen[i].path[j]]];
			if (bienen[i].steps_path == numKnoten - 1)
				[bp closePath];
			[bp stroke];			
		}
	}
	if ([[graph showSingleBee] boolValue]) {
		int i = [[graph singleBeeNumber] intValue];
		if (i >= 0 && i < numBienen) {
			NSBezierPath *bp = [[NSBezierPath alloc] init];
			[bp setLineWidth:scaleX*0.5];
			[[NSColor greenColor] setStroke]; 
			//			[[NSColor colorWithDeviceWhite: 0.5 alpha:0.5] setStroke];
			[bp moveToPoint: [self positionKnoten: bienen[i].path[0]]];
			for	(int j = 1; j <= bienen[i].steps_path; j++)
				[bp lineToPoint: [self positionKnoten: bienen[i].path[j]]];
			if (bienen[i].steps_path == numKnoten - 1)
				[bp closePath];
			[bp stroke];			
		}
	}
	double ks = [graph.knotSize floatValue];
	bool mark_hive = [graph.markHive boolValue];
	for (int i = 0; i < numKnoten; i++) {
		double x = Knoten[i].x;
		double y = Knoten[i].y;		
		NSRect aRect = NSMakeRect(x - 1.5 * scaleX * ks, y - 1.5 * scaleY * ks, ks * 3.0 * scaleX, ks * 3.0 * scaleY);
		[[NSColor blackColor] setStroke];
		if (mark_hive && i == start_node)
			[[NSColor greenColor] setFill];
		else
			[[NSColor blackColor] setFill];
		[NSBezierPath fillRect: aRect];
	}
	if (path_available && [[graph showBestSoFar] boolValue]) {
		NSBezierPath *bp = [[[NSBezierPath alloc] init] autorelease];
		[bp setLineWidth:scaleX*2];
		[[NSColor redColor] setStroke];
		int lengthPath = numKnoten;
		[bp moveToPoint: [self positionKnoten: path_alltimes[0]]];
		for (int j = 1; j < lengthPath; j++)
			[bp lineToPoint: [self positionKnoten: path_alltimes[j]]];
		[bp closePath];
		[bp stroke];
	}
	if (path_available && [[graph showBestOfLastIteration] boolValue]) {
		NSBezierPath *bp = [[NSBezierPath alloc] init];
		[bp setLineWidth:scaleX*1];
		[[NSColor blackColor] setStroke];
		int lengthPath = numKnoten;
		[bp moveToPoint: [self positionKnoten: path[0]]];
		for (int j = 1; j < lengthPath; j++)
			[bp lineToPoint: [self positionKnoten: path[j]]];
		[bp closePath];
		[bp stroke];
	}
}

- (void)updateTransform {
	NSRect knotenArea = (NSRect) {{1E30, 1E30}, {0.0, 0.0}};
	double maxx = -1E30, maxy = -1E30;
	for (int i = 0; i < numKnoten; i++) {
		if (Knoten[i].x < knotenArea.origin.x)
			knotenArea.origin.x = Knoten[i].x;
		if (Knoten[i].y < knotenArea.origin.y)
			knotenArea.origin.y = Knoten[i].y;
		if (Knoten[i].x > maxx)
			maxx = Knoten[i].x;
		if (Knoten[i].y > maxy)
			maxy = Knoten[i].y;
	}
	knotenArea.size.width = maxx - knotenArea.origin.x;
	knotenArea.size.height = maxy - knotenArea.origin.y;
	double edgex = knotenArea.size.width / 20.0;
	double edgey = knotenArea.size.height / 20.0;
	knotenArea.origin.x -= edgex;
	knotenArea.origin.y -= edgey;
	knotenArea.size.width += 2*edgex;
	knotenArea.size.height += 2*edgey;
	NSRect frame = [self frame];
	scaleX = knotenArea.size.width / frame.size.width;
	scaleY = knotenArea.size.height / frame.size.height;
	[self setBounds:knotenArea];
}

- (void)windowDidResize:(NSNotification *)notification {
	[self updateTransform];
}

- (IBAction)displayChange:(id) sender {
	[self setNeedsDisplay:TRUE];
}

- (void)savePanelDidEnd:(NSSavePanel *)sheet returnCode:(int)returnCode  contextInfo:(void  *)contextInfo {
	if (returnCode == NSFileHandlingPanelOKButton)
		[self saveToPDF: [sheet filename]];
}

- (void) saveToPDF: (NSString *) fileName {
	NSRect r = [self bounds];
	r.size.width = r.size.width / scaleX;
	r.size.height = r.size.height / scaleY;	
	NSData *d = [self dataWithPDFInsideRect:r];
	[d writeToFile:fileName atomically:NO];
}


@end
