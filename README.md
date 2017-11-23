
CS32 Winter 2017 with Smallberg

A grade received.

Take inspiration if you want.

Project 3 and 4 are interesting. Please refer to their respective specification PDFs.

### Project 3: Bugs

Interprets code that dictates ant behavior, and runs a simulation.

Introduction (excerpt from spec)

This year, instead of building the traditional video game for CS32 Project #3, you’re going to do something a little different. Not only are you going to build a cool graphical program and learn important Object Oriented Programming (OOP) skills, but you’re also going to create programming competition platform that you can take back to your high-school (should you want to do so) and use to run a simple programming competition. That’s right – your Project #3 is actually a programming competition system, and once you complete it, you can use it to host a programming competition of your own (a hackathon of sorts) where high-school students have to build little programs to control simulated ants that compete against each other. So don’t just think of this Project #3 as a programming project – think of it as an opportunity to excite more folks about the joy of programming, and perhaps change someone’s life trajectory to our exciting field.   

In Project #3, you’re going to build a graphical ant simulation. In your simulation, you’ll place up to four different colonies of simulated ants within a virtual field and allow them to wander around, forage for food, bring the food back to their anthills (where the queen ant will consume the food and then produce more ants), battle with competitor ants and baby and adult grasshoppers, emit pheromones for navigation, and avoid deadly poison. The simulated ant that produces the largest ant colony (by effectively foraging for food and bringing this food back to its anthill, where it can be used to produce more ants) wins the competition.   

Each simulated ant is controlled by its own unique ant program, which the contest entrants can write in a simple language called “Bugs!” that we’ve invented. Your Project #3 program will load one or more of these ant programs (written by the high-school students during the competition), and then use these Bugs programs to run the simulation. The high-school students can watch as their ants try to achieve dominance by growing the biggest colony of ants in the simulation. The ant program that generates the largest number of total ants across the simulation’s twothousand time units (aka ticks) wins. 

### Project 4: Bruin-Nav

Turn-by-turn navigates using the A-star algorithm

Introduction (excerpt from spec)

The NachenSmall Maps & Navigation Corporation, owner of the popular turn-by-turn navigation website WeHaveTheBestNavigationSoftwareInTheWorldIfYouCanJustRememberTheURL.com, has decided to stop licensing a 3rd-party turn-by-turn navigation system (from a company who’s name rhymes with "frugal") and instead build their own navigation engine in-house. Given that the NachenSmall leadership team is comprised entirely of UCLA alums, they’ve decided to offer the job to build a prototype of the new navigation system the students of CS32.  Lucky you! 

So, in your last project for CS32, your goal is to build a simple navigation system that loads and indexes a bunch of Open Street Map geospatial data (which contains latitude and longitude data for thousands of streets and attractions) and then build a turn-by-turn navigation system on top of this data.  Your completed Project 3 solution should be able to deliver optimal directions like the following, which detail how to get from 1031 Broxton Ave. (in Westwood) to The Maltz Park in (Beverly Hills): 

    You are starting at: 1031 Broxton Avenue 
    Proceed 0.09 miles southeast on Broxton Avenue 
    Take a left turn on Kinross Avenue 
    Proceed 0.07 miles east on Kinross Avenue 
    Take a right turn on Glendon Avenue 
    Proceed 0.08 miles southeast on Glendon Avenue 
    Take a left turn on Lindbrook Drive 
    Proceed 0.93 miles east on Lindbrook Drive 
    Take a right turn on Holmby Avenue 
    Proceed 0.08 miles southeast on Holmby Avenue 
    Take a left turn on Wilshire Boulevard 
    Proceed 0.92 miles east on Wilshire Boulevard 
    Take a left turn on Whittier Drive 
    Proceed 0.74 miles north on Whittier Drive 
    You have reached your destination: The Maltz Park 
    Total travel distance: 2.9 miles 
 
If you’re able to prove to NachenSmall’s reclusive and bizarre CEO, Carey Nachenberg, that you have the programming skills to build a simple navigation engine, he’ll hire you to build the full navigation website, and you’ll be rich and famous.

