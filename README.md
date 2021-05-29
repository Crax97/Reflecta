# How does it work?

Check `tests/tests.cpp`. Basically, you declare your class (subclassing `Reflecta::Object` isn't required, 
tho it's usefulf for all the helper methods there).

Then you describe your class using the `REFLECTA_*()` macros (take a look at the tests)