//
//  StartDictReader.h
//  StartDictReader-ios
//
//  Created by Cny on 8/28/13.
//  Copyright (c) 2013 Scorpion. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface StartDictReader : NSObject

@property(nonatomic, readonly) NSString *rpath;	// the start dict folder path.
@property(nonatomic, readonly) NSString *sname;	// the dict file name.
@property(nonatomic, readonly) NSString *idx;
// initial the reader by start dict folder path.
- (id)initWithPath:(NSString *)path name:(NSString *)name;
- (NSString*)find:(NSString *)word;
@end

