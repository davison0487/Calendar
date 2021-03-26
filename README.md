# Calendar
Model/View/Controller (MVC).


## Introduction
This project we will a Calendar that can store events and people for a single year, 2021. Each event will have a name, a date, a time and an attendee for the event.

To populate the calendar, command handlers can understand natural language(-ish) text input. The grammar will be defined below.
The commands to process will be to add, change, delete and show events in the calendar.


### COMMANDS:
- `ADD {event-name}...` -- Keyword indicates "add command" -- create an event on the calendar
- `CHANGE {FIELD} = {NEWVALUE}...` -- Keyword that indicates a "change" command -- to change a calendar event
- `DELETE ...` -- ("or delete") Keyword that indicates a "delete" command -- to remove all events from calendar (that match query)
- `SHOW ...` -- (or "show") Keyword indicates "show command" to select (and see) a list of events based on given arguments
- `QUIT` -- which will cause the application to terminate


### Other KEYWORDS
- `AT/UNTIL` -- Keyword that indicates the time range the event (AT 3:00pm UNTIL 4:30pm)
- `IF` -- Keyword that specifies conditional expression using event property (`IF title='this-title'`), title can be any data memeber of Event(`title`, `date`, `time` or `with`)
- `ON/UNTIL` -- Arguments indicate the event date range (`ON 2021-06-15` | `ON 2021-05-15 UNTIL 2021-05-20`) 
- `WITH` -- Keyword that indicates the person invited to the event (`WITH "bart"`)


### Add Command
Users can add entries to their calendar using natural language. In examples below, CAPITALIZED words indicate keywords in our grammar. Actual natural language statements typed by users for testing do not need to use capital letters -- they can use any case they want.

Example 1: `Add "ECE141 study group" AT 3:00pm UNTIL 4:30pm ON Friday WITH "bart simpson"`

Example 2: `Add "ECE141 study group" ON 2021-03-15 WITH "bart simpson"  AT 3:00pm UNTIL 4:30pm`

Example 3: `Add "WorkShift" ON 2021-06-20 AT 3:00pm UNTIL 4:30pm`


### Change Command
The `CHANGE` command is used to alter events on the calendar.  A single command may change more than one event.

Example 1: `CHANGE title ="my new title" ON 2021-08-04`  -- change title of every event to "new title" on 8/4 

Example 2: `CHANGE title = "new title" with= "jack" IF name="old title" ON 2021-02-10` -- change title and attendee of the event "old title" on 2/10

EXample 3: `IF date = 2021-10-15 CHANGE title = "new title" with = "jack skellington"` -- change title and attendee of every event on 10/15


### Delete Command
The `DELETE` command is used to remove events from the calendar.  

Example 1: `DELETE ON 2021-01-01`  -- delete all events on 1/1

Example 2: `DELETE IF date = 2021-01-01`  -- delete all events on 1/1

Example 3: `DELETE AT 3:00pm UNTIL 4:30pm` -- delete any event (on any day) with matching time range

Example 4: `DELETE AT 3:00pm UNTIL 4:30pm ON 2021-10-30` -- delete any event (on date) with matching time range


## Model/View/Controller" (`MVC`) pattern
`Calendar` and `Event` objects are "models", and `CommandProcessor` classes are controllers. There's also a "chain-of-responsibility" in the main.cpp file.

`views` -- little objects that present results to user.  When user enters a "Show..." command, task will be to obtain the associated set of `Events` from the `Calendar`, and present the results in a text-based table (examples below) "view".  


### Show Events Command
The `SHOW` command is used to create a view -- that contains calendar events for the specified range.  (Users can also just type "SHOW" as an alias).  When a user executes a `SHOW` command, `ShowProcessor` will run as usual, and create an associated `Command` object. Assuming that goes well, the `ShowProcessor::run(Command*)` will be called. Next, query the calendar to gather the associated events, and construct a tabular view (see below) of the events. Once the data is ready to present, call the `View::show(std::ostream &anOutput)` method. 

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
