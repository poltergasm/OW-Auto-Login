## What the hell is this?
If you haven't already guessed from the name, it's an auto login for Overwatch. This means you can run Overwatch without the need of the battle.net launcher, or horrible scripts that use weird hacks in order to send the input to your Overwatch window.

## Limitations
Probably a lot. I imagine it can't handle password that require shift presses just yet. But hey, I'm sure the passwords over at [http://preshing.com/20110811/xkcd-password-generator/] will work just fine!
I think one of the biggest limitations is that 3rd party software like this can't detect when you're actually connected to the Overwatch servers. This means we just have to wait for a period of time before entering the login details. Unfortunately I see no other way around it at the moment.

## Compiling
I've left a binary in the bin folder, but you may need vcdist if you don't already have it. Otherwise, you can compile it yourself with mingw

```
gcc -o login autologin.c
```

As it's all self contained, you can just add a shortcut to the desktop and you're good to go.