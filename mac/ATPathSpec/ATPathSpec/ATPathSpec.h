
#import <Foundation/Foundation.h>


typedef enum {
    ATPathSpecMatchResultUnknown = 0,
    ATPathSpecMatchResultMatched = 1,
    ATPathSpecMatchResultExcluded = -1,
} ATPathSpecMatchResult;

typedef enum {
    ATPathSpecEntryTypeFileOrFolder,
    ATPathSpecEntryTypeFile,
    ATPathSpecEntryTypeFolder,
} ATPathSpecEntryType;

typedef enum {
    ATPathSpecSyntaxOptionsAllowBackslashEscape = 0x01,
    ATPathSpecSyntaxOptionsAllowNewlineSeparator = 0x02,
    ATPathSpecSyntaxOptionsAllowCommaSeparator = 0x04,
    ATPathSpecSyntaxOptionsAllowWhitespaceSeparator = 0x200,
    ATPathSpecSyntaxOptionsAllowPipeUnion = 0x08,
    ATPathSpecSyntaxOptionsAllowAmpersandIntersection = 0x10,
    ATPathSpecSyntaxOptionsAllowParen = 0x20,
    ATPathSpecSyntaxOptionsAllowBangNegation = 0x40,
    ATPathSpecSyntaxOptionsAllowHashComment = 0x80,
    ATPathSpecSyntaxOptionsRequireTrailingSlashForFolders = 0x100,
    ATPathSpecSyntaxOptionsMatchesAnyFolderWhenNoPathSpecified = 0x400,
    ATPathSpecSyntaxOptionsAllowStarWildcard = 0x800,
    ATPathSpecSyntaxOptionsAllowQuestionMarkWildcard = 0x1000,
    ATPathSpecSyntaxOptionsFolderMatchesAllFilesInSubtree = 0x2000,

    // literal string
    ATPathSpecSyntaxFlavorLiteral = 0,
    // shell-style glob, supporting `*` and `?` wildcards
    ATPathSpecSyntaxFlavorGlob = ATPathSpecSyntaxOptionsAllowStarWildcard | ATPathSpecSyntaxOptionsAllowQuestionMarkWildcard,
    // 100% compatibility with `.gitignore`
    ATPathSpecSyntaxFlavorGitignore = ATPathSpecSyntaxFlavorGlob | ATPathSpecSyntaxOptionsAllowBackslashEscape | ATPathSpecSyntaxOptionsAllowNewlineSeparator | ATPathSpecSyntaxOptionsAllowBangNegation | ATPathSpecSyntaxOptionsAllowHashComment | ATPathSpecSyntaxOptionsMatchesAnyFolderWhenNoPathSpecified | ATPathSpecSyntaxOptionsFolderMatchesAllFilesInSubtree,
    // enables all ATPathSpec features
    // note that it's not fully compatible with gitignore because it has ATPathSpecSyntaxOptionsRequireTrailingSlashForFolders flag set
    ATPathSpecSyntaxFlavorExtended = ATPathSpecSyntaxFlavorGitignore | ATPathSpecSyntaxOptionsAllowCommaSeparator | ATPathSpecSyntaxOptionsAllowWhitespaceSeparator | ATPathSpecSyntaxOptionsAllowPipeUnion | ATPathSpecSyntaxOptionsAllowAmpersandIntersection | ATPathSpecSyntaxOptionsAllowParen | ATPathSpecSyntaxOptionsRequireTrailingSlashForFolders,
} ATPathSpecSyntaxOptions;


NSString *ATPathSpecAdjustTrailingSlash(NSString *path, ATPathSpecEntryType type);
NSString *ATPathSpecAddTrailingSlash(NSString *path);
NSString *ATPathSpecRemoveTrailingSlash(NSString *path);


NSString *const ATPathSpecErrorDomain;
NSString *const ATPathSpecErrorSpecStringKey;

typedef enum {
    ATPathSpecErrorCodeInvalidSpecString = 1,
} ATPathSpecErrorCode;


@interface ATMask : NSObject

+ (ATMask *)maskWithString:(NSString *)string syntaxOptions:(ATPathSpecSyntaxOptions)options;

- (BOOL)matchesName:(NSString *)name;

- (NSString *)stringRepresentationWithSyntaxOptions:(ATPathSpecSyntaxOptions)options;

@end


@interface ATPathSpec : NSObject

+ (ATPathSpec *)pathSpecWithString:(NSString *)string syntaxOptions:(ATPathSpecSyntaxOptions)options;
+ (ATPathSpec *)pathSpecWithString:(NSString *)string syntaxOptions:(ATPathSpecSyntaxOptions)options error:(NSError **)error;

+ (ATPathSpec *)emptyPathSpec;

+ (ATPathSpec *)pathSpecMatchingNameMask:(ATMask *)mask type:(ATPathSpecEntryType)type syntaxOptions:(ATPathSpecSyntaxOptions)options;
+ (ATPathSpec *)pathSpecMatchingPathMasks:(NSArray *)masks type:(ATPathSpecEntryType)type syntaxOptions:(ATPathSpecSyntaxOptions)options;
+ (ATPathSpec *)pathSpecMatchingPath:(NSString *)path type:(ATPathSpecEntryType)type syntaxOptions:(ATPathSpecSyntaxOptions)options;
+ (ATPathSpec *)pathSpecMatchingUnionOf:(NSArray *)specs;
+ (ATPathSpec *)pathSpecMatchingIntersectionOf:(NSArray *)specs;

- (ATPathSpec *)negatedPathSpec;

- (ATPathSpecMatchResult)matchResultForPath:(NSString *)path type:(ATPathSpecEntryType)type;
- (BOOL)matchesPath:(NSString *)path type:(ATPathSpecEntryType)type;

- (NSArray *)matchingPathsInArray:(NSArray *)paths type:(ATPathSpecEntryType)type;

- (NSString *)stringRepresentationWithSyntaxOptions:(ATPathSpecSyntaxOptions)options;
- (NSString *)description; // gives a string representation in Extended syntax

@end
