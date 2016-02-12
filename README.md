# BigPictureWrapper
Fix for recent issues with launching Steam Big Picture mode through NVIDIA GameStream.

# Usage
This fix consists of two executables, which happen to be exactly the same: `BigPictureWrapper.exe` and `BigPictureService.exe`.  The need for two arised out of the technique used to terminate the NVIDIA GameStream after Big Picture mode is exited.

Place both programs in the same folder and run BigPictureService before you do anything.  It will require UAC elevation (admin rights), which is only needed because the NVIDIA Curtain (the window that takes up the whole screen and has the NVIDIA GameStream logo on it while the game is launching) runs under the SYSTEM account and therefore can't be closed/killed without admin rights.  The BigPictureService program will run in the background and there actually isn't any indication that it is running (you can check Task Manager to verify).

Next, you will need to add the BigPictureWrapper program as a game in GeForce Experience.  Open GeForce Experience (either from the Start menu or the icon in the Notificaton Area) and go to Preferences -> SHIELD.  Click on the + icon on the right-hand side and locate BigPictureWrapper.exe.  Once you have added it, you can double-click the new entry and change the name to something like "Steam Big Picture" or whatever you wish.  Now when launching a game through NVIDIA GameStream you can select this new shortcut instead of the normal Steam shortcut.

I have verified that this works for both non-beta Steam and beta Steam as of this writing (2016-02-11).
