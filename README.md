# ECE141-wi21-assignment4
### Due March 2, 2021 @ 11:30pm

## Introduction
This assignment will build on the ideas of Parsing from Assignment 3, and further explore the concepts we discussed related to Model/View/Controller (MVC).

You'll build a Calendar that can store events and people for a single year, 2021. Each event will have a name, a date, a time and an attendee for the event.

To populate the calendar, you will implement command handlers that can understand natural language(-ish) text input. The grammar will be defined below.
The commands to process will be to add, change, delete and show events in the calendar.

Good luck!

##  Given --

1.  We provide command-line driven system to gather user input (commands)
2.  We provide parser class to convert characters to tokens (like in assignment #3)
3.  We provide an input tokenizer (which tokenizes all input in a single pass, in advance)
4.  We provide a basic `Calendar` and `Event` classes (you'll finish the implementation).
5.  Dates will be in a fixed format ("2021-01-04")
6.  Times will also be in a fixed format ("3:00pm") -- presume your local timezone 

The system should compile for you without changes -- but it won't pass the tests until you implement your parts.
 
## Challenge #1
- Using the given `Calendar` and `Event` classes, complete the implementation to meet your needs.  Your `Calendar` class is going to need methods that provide functionality used by "add", "remove", and "find" commands (we defined add and remove for you already - but you must implement them).  You can make any changes you like to the `Calendar` and `Event` classes, except where noted in the code.  These classes are meant to be simple, because the main focus of this assignment relates to parsing and processing "english-like" sentences.  

This challenge is meant to be straightforward -- as managing events on the calendar is necessary for other challenges. The `Event` class is meant to be as simple as possible to hold event-related information.

### `Event`
```
class Event {
  std::string title;
  std::string date;
  std::string time;
  std::string with;
}
```

### `Calendar`
An object that contains N events for the year 2021.
```
class Calendar {
 
  bool addEvent(Event *anEvent);
  bool removeEvent(Event *anEvent);
    
 //add your own method(s) here...

  friend class TestAutomatic; //REQURED for autograder..

  std::vector<Event*> events;   //REQUIRED - do not change...
};
```


## Challenge #2

In this challenge, you're going to implement the `CommandProcessor` objects (one each for 'add','change','delete','show'). Each of these objects is aggregated into a "chain-of-responsibility" in the `main.cpp` file.  We've already implemented the 'quit' command processor and its associated command. We've provided a baseline for your others as well (`AddProcessor`, `ChangeProcessor`, `DeleteProcessor`, `ShowProcessor`) -- that you need to fully implement.  

### Step 2.1 

First -- you need to teach your command(s) how to analyze a given set of tokens (from the given `Tokenizer` object), and determine if the tokens represent a command that it can handle. 

Implement the `recognizes(Tokenizer &aTokenizer)` method for each command (in the associated `XXXProcessor` files). In this method, you have access to the `Tokenizer`, and its list of `Tokens` that were tokenized from the user input. Examine the given `Tokens` in the `Tokenizer` and decide if they represent a valid command for the given `CommandProcessor` class (add, delete, change, show). Return `true` if a given `CommandProcessor` can take responsibility for a given set of tokens that represent the associated command.

Take a moment to review the interface provided by the `Tokenizer` class. It offers lots of ways to iterate the underlying list of tokens, that you can use as you're analyzing the tokenized use input.

### Step 2.2 

Next -- presuming a given `XXXXProcessor` has identified a given set of tokens it can handle, now you need to actually analyze, interpret, and respond to that command. Begin this phase by implementing the `makeCommand` method for each of your `CommandProcessor` classes (add, change, delete, show). The `makeCommand` method will create a new `Command` object from the given set of tokens. You can change the `Command` class to hold any data you want, or make subclasses on a per-command basis. The `Command` will hold whatever state is necessary (including tokens from user input) so that you have the information you need to perform the associated action (in Step 2.3).

> In examples below, CAPITALIZED words indicate keywords in our grammar. Actual natural language statements typed by users for testing can use any case they want, and are likely to be lowercase.

### Step 2.3

Once a `Command` object has been created for a given set of tokens, the associated `CommandProcessor` object will be asked to `run(Command*)` the given `Command` and perform the associated action. Implement the `run` method for each of your `CommandProcessors` -- (add, change, delete, show).  For "add", "change", "remove", the `run` method will alter the given `Calendar` object in some way (adding, changing, removing events).  For `show` commands, you will create a view that presents a set of matching events (more on this later).  The `quit` command has already been implemented for you, and simply termniates the running of the interactive mode of your interpreter.

### COMMANDS:
- `ADD {event-name}...` -- Keyword indicates "add command" - we should create an event on the calendar
- `CHANGE {FIELD} = {NEWVALUE}...` -- Keyword that indicates a "change" command -- to change a calendar event
- `DELETE ...` -- ("or delete") Keyword that indicates a "delete" command -- to remove all events from calendar (that match query)
- `SHOW ...` -- (or "show") Keyword indicates "show command" to select (and see) a list of events based on given arguments
- `QUIT` -- which will cause the application to terminate  (provided for you)

### Other KEYWORDS

- `AT/UNTIL` -- Keyword that indicates the time range the event (AT 3:00pm UNTIL 4:30pm)
- `IF` -- Keyword that specifies conditional expression using event property (`IF title='this-title'`), title can be any data memeber of Event(`title`, `date`, `time` or `with`)
- `ON/UNTIL` -- Arguments indicate the event date range (`ON 2021-06-15` | `ON 2021-05-15 UNTIL 2021-05-20`) 
- `WITH` -- Keyword that indicates the person invited to the event (`WITH "bart"`)

Your code will also understand names of event properties (`title` | `date` | `time` | `with`)

### Add Command

The `AddProcessor` class already exists, but you'll need to implement the methods as described above. You'll also need a `Command` class (used by the `AddProcessor`) to captures the necessary state from the tokenizer (tokens) that you need to `run` the "Add" command and alter the given `Calendar` accordingly.

Users can add entries to their calendar using natural language. In examples below, CAPITALIZED words indicate keywords in our grammar. Actual natural language statements typed by users for testing do not need to use capital letters -- they can use any case they want.

Example 1: `Add "ECE141 study group" AT 3:00pm UNTIL 4:30pm ON Friday WITH "bart simpson"`

Example 2: `Add "ECE141 study group" ON 2021-03-15 WITH "bart simpson"  AT 3:00pm UNTIL 4:30pm`

Example 3: `Add "WorkShift" ON 2021-06-20 AT 3:00pm UNTIL 4:30pm`


> Notice that in our example, the parts of things in the commands are reordered

### Change Command

The `ChangeProcessor` class already exists, but you'll need to implement the methods as described above. You'll also need a `Command` class (created by the `ChangeProcessor`) to capture the necessary state from the tokenizer (tokens) that you need to `run` the "Change" command and alter the given `Calendar` accordingly. You also need to #include and register the `ChangeProcessor` in the "chain-of-responsibility" list in `TestManually.hpp`.

The `CHANGE` command is used to alter events on the calendar.  A single command may change more than one event.

Example 1: `CHANGE title ="my new title" ON 2021-08-04`  -- change title of every event to "new title" on 8/4 

Example 2: `CHANGE title = "new title" with= "jack" IF name="old title" ON 2021-02-10` -- change title and attendee of the event "old title" on 2/10

EXample 3: `IF date = 2021-10-15 CHANGE title = "new title" with = "jack skellington"` -- change title and attendee of every event on 10/15

> Notice that in example 3, the keyword `CHANGE` is in the middle of the command


### Delete Command

The `DeleteProcessor` class already exists, but you'll need to implement the methods as described above.  You'll also need a `Command` class (created by the `DeleteProcessor`) to capture the necessary state from the tokenizer (tokens) that you need to `run` the "Delete" command and alter the given `Calendar` accordingly. You also need to #include and register the `DeleteProcessor` in the "chain-of-responsibility" list in `TestManually.hpp`.

The `DELETE` command is used to remove events from the calendar.  

Example 1: `DELETE ON 2021-01-01`  -- delete all events on 1/1

Example 2: `DELETE IF date = 2021-01-01`  -- delete all events on 1/1

Example 3: `DELETE AT 3:00pm UNTIL 4:30pm` -- delete any event (on any day) with matching time range

Example 4: `DELETE AT 3:00pm UNTIL 4:30pm ON 2021-10-30` -- delete any event (on date) with matching time range


## Challenge #3

We previously discussed the "Model/View/Controller" (`MVC`) pattern in lectures for this course. In this assignment, your `Calendar` and `Event` objects are "models", and your `CommandProcessor` classes are controllers. There's also a "chain-of-responsibility" in the main.cpp file.

In this challenge, you're going to implement "views" -- little objects that present results to your user.  When your user enters a "Show..." command, your task will be to obtain the associated set of `Events` from the `Calendar`, and present the results in a text-based table (examples below) "view".  Your `View` class should implement a `show(std::ostream &anOutput)` method to present the data to the user. The `CommandProcessor` for the "show" command should create a view with data from the `Calendar`, and call its `show` method.

> NOTE:  Make as many "view" classes as necessary; they must implement the `IView` interface defined in `IView.hpp`.

### Show Events Command

The `ShowProcessor` class already exists, but you'll need to implement the methods as described above.  You'll also need a `Command` class (created by the `ShowProcessor`) to capture the necessary state from the tokenizer (tokens) that you need to `run` the "Show" command and alter the given `Calendar` accordingly. You also need to #include and register the `ShowProcessor` in the "chain-of-responsibility" list in `TestManually.hpp`.

The `SHOW` command is used to create a view -- that contains calendar events for the specified range.  (Users can also just type "SHOW" as an alias).  When a user executes a `SHOW` command, your `ShowProcessor` will run as usual, and create an associated `Command` object. Assuming that goes well, the `ShowProcessor::run(Command*)` will be called. 

Next -- query the calendar to gather the associated events, and construct a tabular view (see below) of the events. Once you're ready to present the data, call the `View::show(std::ostream &anOutput)` method. ALWAYS output your view data to the stream provided to your `Calendar` object during construction, by passing the `Calendar.stream` member to the `View::show()` method.

> Note: Data should be ordered by date (as needed). Ordering by time is not expected, but you're welcome to add that if you like.

Example1: `SHOW ON 2021-02-18` -- show list of events on given date...

```
|---------------|---------------|------------------|-------------------|
| Title         | Date          | Time             | With              |
|---------------|---------------|------------------|-------------------|
| Study group   | 2021-02-18    | 10:00am-11:00am  | Bart Simpson      |
| Interview     | 2021-02-18    | 11:00am-1:00pm   | Mr. Barnes        |
| Online quiz   | 2021-02-18    | 3:00pm-3:30pm    |                   |
|---------------|---------------|------------------|-------------------|
```

EXample 2: `SHOW IF title = "Study group"`  -- show all events on calendar with given name

```
|---------------|---------------|-------------------|-------------------|
| Title         | Date          | Time              | With              |
|---------------|---------------|-------------------|-------------------|
| Study group   | 2021-02-18    | 10:00am-11:00am   | Lisa Simpson      |
|---------------|---------------|-------------------|-------------------|
```

Example 3: `SHOW`  -- show all events on calendar

Example 4: `SHOW ON 2021-05-01`  -- show events for a date (where no events exist)
```
No scheduled events found.
```


## Turning in your work
### Due March 2, 2021 @ 11:30pm

#### Grading Rubric 

- Compile Test : 5%
- Add Test: 15%
- Change Test: 15%;
- Delete Test: 10%
- Show Test: 10%
- Stress Test: 40%
- Code style and comments: 5%



Good luck everyone!

