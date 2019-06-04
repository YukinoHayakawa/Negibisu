# Negibisu

## Introduction

A markup lanugage for visual novels. The syntax should allow the story writer to incorporate common operations into the text of the story without compromising too much of the text readability.

## Features

- [ ] Chapters and sections can be nested
- [ ] Each line is a line of game dialog
- [ ] A line of dialog can be associated with a speaking character
- [ ] Commands can be inserted within dialog texts
- [ ] The text can be annotated for editing purpose
- [ ] Allow using variables to store and pass command parameters
- [ ] Allow state restoration and arbitrary location preview
- [ ] Text characters can be escaped by prefixing a slash (\\)
- [ ] Use ASCII code 7 (beep) to set breakpoints in the scripts (need to set corresponding breakpoints in the debugger, too)


## Script Structure



## Lexical Grammar

The lexical grammars are context-sensitive as in many lanugages. The analyser may parse the text using different rules according to neighbor tokens.

### Token Types (In Regular Expressions)

```
               Sharp -> ^[\s#]*#+ (# being the first non-whitespace character of a line or following another #)
         EscapedChar -> \.
             NewLine -> \n
               Colon -> :
               Comma -> ,
         LeftBracket -> [
        RightBracket -> ]
           LeftBrace -> {
          RightBrace -> }
     LeftDoubleBrace -> {{
    RightDoubleBrace -> }}
       StringLiteral -> <Character sequence depending on the context>
```
### Lexical Analysis Contexts

```
GLOBAL
TITLE
COMMENT
COMMAND
```
## Syntactic Grammar

A grammar suitable for a recursive descent parser.

```
           Script -> Sections
         Sections -> Section | nil
          Section -> Title Content
            Title -> # StringLiteral : StringLiteral
          Content -> Line NewLine Content | nil
             Line -> CharacterTag Dialog | Dialog
     CharacterTag -> [StringLiteral] | [StringLiteral , StringLiteral , StringLiteral]
           Dialog -> Command | StringLiteral | StringLiteral Dialog
          Command -> { StringLiteral CommandArgs }
      CommandArgs -> : CommandArgSeq | nil
    CommandArgSeq -> StringLiteral CommandArgSeq | nil
```

## Semantic Rules


## Code Generation


## Available Commands

### 


## Code Samples

