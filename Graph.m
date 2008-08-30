/*
 * Graph.m
 * Implementierung Bienenalgorithmus
 *
 * $HeadURL: svn://localhost/nadia/diplomarbeit/software/Graph.m $
 * $Id: Graph.m 220 2008-08-17 09:48:14Z kthul $
 */

#import "Graph.h"
#import "bienen.h"
#import "globals.h"

@implementation Graph

- (id) init {
	self = [super init];
	if (self) {
		parameter_B_max = [NSNumber numberWithInt:10];
		parameter_z_max = [NSNumber numberWithInt:10];	
		parameter_w = [NSNumber numberWithFloat:0.5];
		parameter_b = [NSNumber numberWithInt:5];
		parameter_p_star = [NSNumber numberWithFloat:0.05];
		parameter_p = [NSNumber numberWithFloat:0.7];
		parameter_theta = [NSNumber numberWithFloat:0.3];
		parameter_a = [NSNumber numberWithFloat:0.001];
		parameter_z = [NSNumber numberWithInt:0];
		use2Opt = [NSNumber numberWithBool:TRUE];
		use2OptE = [NSNumber numberWithBool:TRUE];	
		parameter_u = [NSNumber numberWithInt:1];
		knotSize = [NSNumber numberWithFloat:1.0];
		markHive = [NSNumber numberWithBool:NO];
	}
	return self;
}

- (IBAction) loadGraph: (id) sender {
	NSOpenPanel *op = [NSOpenPanel openPanel];
	[op runModalForDirectory:nil file:nil types:nil];
	NSArray *fns = [op filenames];
	for (NSString *fn in fns) {
		if (loadGraph([fn UTF8String]))
			return;
	}
	[graphView updateTransform];
	[graphView setNeedsDisplay: YES];
	[self reset: sender];
}

- (void) doIterations:(int)num sender: (id) sender {
	if (numKnoten > 0) {
		int u = [parameter_u intValue];
		if (u != 1) {
			for (; u <= numKnoten; u++)
				[self stage:self];
		}
		for (int i = 0; i < num; i++) {
			iteration(
				[parameter_B_max intValue],
				[parameter_w floatValue],
				[parameter_b floatValue],	
				[parameter_p_star floatValue],
				[parameter_p floatValue],
				[parameter_theta floatValue],
				[parameter_a floatValue],
				[parameter_z intValue],
				[use2Opt boolValue],
				[use2OptE boolValue]
			);
			self.parameter_lopt = [NSNumber numberWithDouble:lopt];
			self.parameter_z = [NSNumber numberWithDouble: [self.parameter_z floatValue] + 1];
			[graphView setNeedsDisplay: YES];
		}
	} else
		NSBeginAlertSheet(
			@"There is no graph loaded yet", @"OK", nil, nil, window, self, nil, 
			nil, sender, @"Please select load to read graph."
		);
}

- (IBAction)iteration:(id)sender {
	[self doIterations: 1 sender: sender];
}

- (IBAction)iterations:(id)sender {
	[self doIterations: [parameter_z_max intValue] sender: sender];
}

- (IBAction)stage:(id)sender {
	if (numKnoten > 0) {
		int new_u = stage(
			[parameter_u intValue],
			[parameter_B_max intValue],
			[parameter_w floatValue],
			[parameter_b floatValue],	
			[parameter_p_star floatValue],
			[parameter_p floatValue],
			[parameter_theta floatValue],
			[parameter_a floatValue],
			[parameter_z intValue],
			[use2Opt boolValue],
			[use2OptE boolValue]
		);
		if (new_u == 1) {
			self.parameter_lopt = [NSNumber numberWithDouble:lopt];
			self.parameter_z = [NSNumber numberWithDouble: [self.parameter_z floatValue] + 1];
		}
		self.parameter_u = [NSNumber numberWithInt:new_u];
		[graphView setNeedsDisplay: YES];
	} else
		NSBeginAlertSheet(@"There is no graph loaded yet", @"OK", nil, nil, window, self, nil, 
		 nil, sender, @"Please select load to read graph.");
}

- (IBAction)print:(id)sender {
	NSSavePanel *sp = [NSSavePanel savePanel];
	[sp setRequiredFileType:@"pdf"];
	[sp setTitle:@"Save graph as PDF file"];
	[sp beginSheetForDirectory:nil file:nil modalForWindow:[sender window] modalDelegate:graphView 
				didEndSelector:@selector(savePanelDidEnd:returnCode:contextInfo:) contextInfo:nil];
}

- (IBAction)pdfSeries:(id)sender {
	NSSavePanel *sp = [NSSavePanel savePanel];
	[sp setRequiredFileType:@"pdf"];
	[sp setTitle:@"Save iteration as series of PDF files"];
	[sp beginSheetForDirectory:nil file:nil modalForWindow:[sender window] modalDelegate:self 
				didEndSelector:@selector(pdfSeriesDidEnd:returnCode:contextInfo:) contextInfo:nil];
}


- (void)pdfSeriesDidEnd:(NSSavePanel *)sheet returnCode:(int)returnCode  contextInfo:(void  *)contextInfo {
	if (returnCode != NSFileHandlingPanelOKButton)
		return;
	NSString *extension = [[sheet filename] pathExtension];
	NSString *name = [[sheet filename] stringByDeletingPathExtension];
	int u;
	do {
		u = [parameter_u intValue];
		for (int i = 0; i < numBienen; i++) {
			self.showSingleBee = [NSNumber numberWithBool:YES];
			self.singleBeeNumber = [NSNumber numberWithInt:i];
			[graphView setNeedsDisplay: YES];
			[graphView saveToPDF: [NSString stringWithFormat:@"%@_%d_%d.%@", name, u, i, extension]];
		}
		[self stage:self];
	} while(u < numKnoten);
}

- (IBAction)reset:(id)sender {
	self.parameter_z = [NSNumber numberWithDouble:1];
	self.parameter_u = [NSNumber numberWithInt:1];
	self.parameter_lopt = nil;
	initialize();
	[graphView setNeedsDisplay: YES];
}

- (IBAction)test:(id)sender {

}

@synthesize parameter_B_max, parameter_z_max, parameter_w, parameter_b, parameter_p_star,
 parameter_p, parameter_theta, parameter_a, parameter_lopt, parameter_z, parameter_u,
 showAllBees, showBestSoFar, showBestOfLastIteration, showLinkStrength, use2Opt, use2OptE,
 showSingleBee, singleBeeNumber, knotSize, markHive;
 
@end
