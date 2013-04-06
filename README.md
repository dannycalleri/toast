Toast Framework
===============

Toast Framework is a 2D easy-to-use C++ game framework inspired by FlashPunk. 

What's that?
--------------------------------------
If you already know AS3 and FlashPunk, Toast Framework is perfect
to make your projects run natively on Windows, Mac OS X and Linux. Yep! 
Toast Framework is multi-platform and it could also be ported on iPhone.

It's perfect if you want to switch from AS3 to C++ in the painless way
possible.

It's perfect for learning C++ while doing what you love: making games.

It already has most of the features needed to complete a nice little game, but
you are encouraged to help and share your code with the others. 

How-to (Windows)
--------------------------------------
First thing you need is Visual Studio 2012:
http://www.microsoft.com/visualstudio/eng/downloads

Visual Studio Express 2012 is perfect, just remember to install the Desktop edition.

Toast is also based on Allegro 5.0.8, so you need to download the library here:
http://cdn.allegro.cc/file/library/allegro/5.0.8/allegro-5.0.8-msvc-11.0.zip

Now follow these easy steps:

1. Create a new directory in C, I called mine "coding"
2. Unzip the file allegro-5.0.8-msvc-11.0.zip inside this directory
3. Check out Toast or download it as a zip inside the same directory and unzip it
4. Now you should have a directory C:\coding in which you have two directories, one called "allegro-5.0.8-msvc-11.0" and another one called "toast-master" or something like this.
5. Go to C:\coding\toast-master\ToastFramework and open the file ToastFramework.sln with VS2012
6. Right mouse click on the "ToastFramework" solution in the Solution Explorer on the left and choose "Build". Now take a cup of coffee (or tea) while VS builds it, but don't worry, it should only take a moment.
7. Right mouse click on the "DEMO" solution and choose "Set as StartUp project" just to make sure you'll execute the game and not the generated .lib file for the framework (that however is not possible!)
8. Use the provided DEMO solution to bootstrap your game project! 
9. Don't forget to put allegro's bin directory (C:\coding\allegro-5.0.8-msvc-11.0\bin) in the Windows path, otherwise when you try to execute the game it will give you an error regarding a missing dll file. Please follow this guide to update your system's PATH variable: http://msdn.microsoft.com/it-it/library/ee537574(v=office.14).aspx 
10. Enjoy!

I 

Contribution
--------------------------------------
The code is thoroughly commented, but if you run into any problem, let me know! :)

In the spirit of open source software development, I always encourages community code contribution, so let's make a fork!
Just please let me know if you do something interesting with it.

License
--------------------------------------
[MIT License](http://opensource.org/licenses/mit-license.php)

&copy; 2013 Danny Calleri &lt;daniele.calleri@gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, see the accompanying "MIT-LICENSE.txt" file.
