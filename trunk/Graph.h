/*
 * Graph.h
 * Control class of the GUI
 *
 * $HeadURL$
 * $Id$
 */

#import <Cocoa/Cocoa.h>
#import "GraphView.h"

@interface Graph : NSObject {
	IBOutlet GraphView *graphView;
	IBOutlet NSWindow *window;
	NSNumber *parameter_B_max;
	NSNumber *parameter_z_max;	
	NSNumber *parameter_w;
	NSNumber *parameter_b;	
	NSNumber *parameter_p_star;
	NSNumber *parameter_p;		
	NSNumber *parameter_theta;
	NSNumber *parameter_a;
	NSNumber *parameter_lopt;
	NSNumber *parameter_z;
	NSNumber *parameter_u;
	NSNumber *showAllBees;
	NSNumber *showBestSoFar;
	NSNumber *showBestOfLastIteration;
	NSNumber *showLinkStrength;
	NSNumber *showSingleBee;
	NSNumber *singleBeeNumber;
	NSNumber *use2Opt;
	NSNumber *use2OptE;
	NSNumber *knotSize;
	NSNumber *markHive;
}

@property(copy) NSNumber *parameter_B_max;
@property(copy) NSNumber *parameter_z_max;
@property(copy) NSNumber *parameter_w;
@property(copy) NSNumber *parameter_b;
@property(copy) NSNumber *parameter_p_star;
@property(copy) NSNumber *parameter_p;
@property(copy) NSNumber *parameter_theta;
@property(copy) NSNumber *parameter_a;
@property(copy) NSNumber *parameter_lopt;
@property(copy) NSNumber *parameter_z;
@property(copy) NSNumber *parameter_u;
@property(copy) NSNumber *showAllBees;
@property(copy) NSNumber *showBestSoFar;
@property(copy) NSNumber *showBestOfLastIteration;
@property(copy) NSNumber *showLinkStrength;
@property(copy) NSNumber *use2Opt;
@property(copy) NSNumber *use2OptE;
@property(copy) NSNumber *showSingleBee;
@property(copy) NSNumber *singleBeeNumber;
@property(copy) NSNumber *knotSize;
@property(copy) NSNumber *markHive;

- (IBAction)loadGraph:(id)sender;
- (IBAction)iteration:(id)sender;
- (IBAction)iterations:(id)sender;
- (IBAction)stage:(id)sender;
- (IBAction)print:(id)sender;
- (IBAction)reset:(id)sender;
- (IBAction)pdfSeries:(id)sender;

@end
