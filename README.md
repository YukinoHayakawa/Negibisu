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



## Script Structure



## Lexical Grammars

### Token Types (In Regular Expressions)

```
         SectionMark -> #+
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
          Identifier -> [a-zA-Z0-9]+
       StringLiteral -> <Any character sequence that is not control character or newline>
```

### Lexical Analyser Pesudocode

```c++
void tokenize(stream in)
{
    while(in)
    {
        switch(nextChar())
        {
            
        }
    }
}
```

## Syntactic Grammars

SectionTitle -> \#+ Text:Text

Command -> {}

## Semantic Rules


## Code Generation


## Available Commands

### 


## Code Samples

