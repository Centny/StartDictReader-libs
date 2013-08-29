//
//  StartDictReader.m
//  StartDictReader-ios
//
//  Created by Cny on 8/28/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import "StartDictReader.h"

@implementation StartDictReader
- (id)initWithPath:(NSString *)path name:(NSString *)name;
{
	self = [super init];

	if (self) {
		_rpath	= path;
		_idx	= [NSString stringWithFormat:@"%@/%@.idx", path, name]
	}

	return self;
}
- (NSString*)find:(NSString *)word{
    NSInputStream *input=[[NSInputStream alloc]initWithFileAtPath:self.idx];
    
    return nil;
}
@end

