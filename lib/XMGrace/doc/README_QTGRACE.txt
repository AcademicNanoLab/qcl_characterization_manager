General Notes on Version 0.2.6:

This release v0.2.6 contains several additions especially concerning the user interface. I tried to fit the changes in while keeping as much of the Grace-interface unchanged as possible. In some cases I used switches in the preferences where the user can activate or deactivate new features in some other cases I just added new options or had to do some minor rearrangements. I hope this is not too much of an inconvenience to anybody.

New Features:
- The most obvious change (since it should become visible with the first start of the new version of QtGrace v0.2.6) is the existence of a Setup-Wizard-Dialog. This new dialog is meant to introduce and explain some of the new features of QtGrace - new in comparison with Grace and new in comparison with earlier versions of QtGrace. The Wizard lets you make all relevant settings, but you have to go throught all pages of the dialog to save the settings. If you do not have the time to complete the dialog, you can later open it via Preferences->Miscellaneous->Summon setup Wizard. All settings in the Wizard are also present in the preferences (but with less explanation).
- In Grace as well as in previous versions of QtGrace the set appearance is set in the corresponding dialog. If you select multiple sets, change something and press Apply, then all settings are applied to all selected sets in the same way (all sets look the same afterwards). This is still the default behavior of QtGrace. But now there is also an option in the set-appearance to deactivate this. "Apply all settings" is active as default, but can now be deactivated to just apply the settings you actually changed. This is useful if you have already set up everything and just want to change a small thing (like the linewidth) for multiple sets without wanting to alter everything else.
- The text in the main drawing area can now be edited directely in the main drawing area. This has to be activated in the preferences. To edit something double-click on it, enter the new text and press return. Pressing Esc or clicking outside the text will cancel the text-editing. In Grace double-clicking opens the corresponding dialog where the text can be set. If you want this while the direct editing is activated, hold Ctrl while double-clicking.
- In set lists there is now a display option to show small icons representing the set apperance. This makes it easier to identify different sets in the list. This option as well as the other display options for set lists can be set in the preferences as well as for every set list individually.
- Sets and graphs can be highlighted. This is useful if you have lots of sets and want to know which set-id corresponds to which set. To highlight something select it in a list and press 'h'. To stop the highlighting press 'h' again. There is also an option to automatically stop the highlighting after a predefined amount of time. The highlighting is done by drawing the set (or graph-frame) with increased linewidth and a slashed line in a predefined color (red by default).
- In the context menu for sets I included three sub-menus to be able to set the appearance of a set from the menu. This is especially useful for the next new feature.
- I made it possible to summon a set-context-menu when right-clicking on a set(-point) in the main drawing area. This is the same menu as can be summoned from every set list. The set is highlighted to indicate which one has been selected.
- For all colors there is now a setting for the transparency (alpha-channel). In the preferences you can select where to show the setting for it: right of every color selector or inside the color-selector-popup. To change the transparency click on the rectangle and use the slider. If the transparency selector is set to be hidden, the transparency of every color will be ignored! Please note that the transparency is not compatible with Grace and that not every output-format supports the alpha-channel.
- There can now be activated two more buttons in the tool-bar: one for Pan and one for Move. Pan lets you drag the viewing area in a graph around (click-drag-release). Move lets you do Pan but also move everything around that can be clicked on (objects, legends, Text). The Move-operation can be a bit dangerous if used without care, therefore I left the Pan-button in because it is much saver. Both buttons have to be activated in the preferences. You may also set the buttons als always on, which means that no button is displayed, but the button is concidered to be always pressed.
- The size of the page of a project can now be changed by double-clicking in the bottom right hand corner of the drawing area. Then you have to drag and click again to finish the resizing.
- There is now an option in the set-apperance-dialog that activates error-bars inside legends (in the "Error bars"-tab). (This setting is not compatible with Grace.)
- There is also a further option for the errorbars of a set: you can choose to connect the error-bars. This lets a set with error-bars look like a tube. You can for example use this to indicate the allowed limits for some measurements if you want to compare actual measurements with the limits.
- There is also an option in the set-apperance-dialog (in the "Main"-tab) that lets autoscale ignore a set during autoscaling. This is useful, if you have a set with much larger values than all other sets in the graph. This is intended for lines that are meant as a reference (like an upper limit). Just set the values much bigger than any other expected values and let this set be ignored during autoscaling. (This setting is not compatible with Grace.)
- For dialogs that have a source- and a destination-group for selecting graph and set(s) there is now a "Sync.selection"-option in the desination group. If this option is activated, the destination group will always have the same graphs and sets selected as the source group.
- There is now an option to increase the size of the buttons in the tool bar (Preferences->QtGrace-GUI->Tool bar size).
- There is now a new type of Graph: a new polar plot. The new polar plot has extended options for setting the angles and radius. Negative values for the radius are possible (but I do not know whether this is actually useful). It is possible to set minimal values for the radius and to set a central area without a plot in it to indicate that a radius-range is missing. You may also set a position for the angle=0. Usually it is at the 3-o-clock-position, but can now be set everywhere. The autoscaling functions revert the angle to 0-2Pi als the time (regardless of what the actual limits of the angle are)! You can still set different angles manually, but autoscale will redisplay a full circle. There are also better options for the axis-labels and tick-labels than for the usual polar plot.
- The mouse-wheel can now be used to zoom in and out in a graph. To do so you have to activate the corresponding option in Preferences->QtGrace-Behavior. If you want to zoom just the X-Axis hold down the Alt-Key while using the mouse-wheel. For zooming the Y-Axis only hold down the Crtl-Key.
- The mouse-wheel can also be used to change the size of objects (like boxes or lines). You may also change the font sizes of all text. To enable this feature activate the option in Preferences->QtGrace-Behavior. To change the font size of the axis-tick-labels use the mouse-wheel on the axis itself. For all other texts just use the mouse-wheel on the text itself.
- The positions of title and subtitle of every graph can now be adjusted. To do so change the values of x-shift and y-shift in the graph-appearance dialog.
- I included an alternative control for the viewport stack. In Preferences->QtGrace-GUI there is an option to select the controls. You can choose the kind of controls you like: Either no viewport-stack-controls ("Hide controls"), the controls of Grace ("Simple controls") or the new controls ("Complete list"). The Grace-controls work with push and cycle for the stack. The QtGrace-controls allow more control over the stack: you can alter the position in the stack, you can name every viewport in the stack, you can replace a viewport in the stack and you can access any viewport as you wish (not only cycle through). Cycling through the stack is possible in both directions.
- In Grace as well as in previous versions of QtGrace antialiasing can only be used for text. Now there is also an option for antialising all drawn objects (apart from text). To activate this feature go to "View->page setup". The antialiasing for objects may look better or worse than the normal drawing mode (you will have to see for yourself). Please not, that for some export formats the antialiasing is part of the viewer (like pdf) and can not be set in the exported file itself.
- If you delete a set from a list of sets you have a gap in the ids in the list. To prevent this, there is a function called "Pack all sets" which can be accessed via the set-popup-menu. This function was also present in Grace. Now there is an option to always pack the sets after something was deleted. To activate this go to Preferences->QtGrace-Behavior and check "Auto-pack sets". Be warned: Please be careful with this option, changing set-ids is maybe not something you constantly want!
- There is now an option to use more icons for the buttons. In Preferences->QtGrace-Behavior you can select ("Icons on buttons/in lists") whether on some of the buttons (in the toolbar and in the drawing-objects-dialog) will be text only or icons only or text and icons. This effects also color selectors and symbol type selectors.


Changes:
- The Error-bars of sets can now be taken into account when using the autoscale function. This makes sure, that error-bars do not extend outside the viewing area of a graph. To activate this feature go to Preferences->QtGrace-Behavior and activate "Consider error bars for autoscale". This makes sure, that the errorbars are not outside the graph, but the riser-bars (or the set-points itself for sets without error bars) can still lie on the axis-lines and be therefore somewhat invisible. To make sure, that there is a small area between the last point or riser-bar you can also set a small amount of the drawing area near the borders to be left empty when autoscale is used. In the axis-dialogs you can still set any values for the axis-limits you like. The autoscale offset (as found in Preferences->QtGrace-behavior) is just used for autoscaling.
- When increasing the size of a graph via double-clicking on the graph corners you can now not go closer to the border of the drawing area than 5% of the width or height of the drawing area. This is to prevent that you can not reach the corners again. Keep in mind, that you still can set to graph dimensions to anything you like via the Graph-appearance-dialog.
- In the context menu for sets the different options at the end of the menu have been split into three sub-menus: "Display operations" to set what is shown in the set-list, "Selector operations" to select different sets (like all even, all invisible, and so on), "Set operations" to apply some simple set operations (like sorting, restricting, and so on)
- If in a file dialog a filename is entered and the directory changed afterwards the filename is kept now (in previous versions the filename was lost when the directory was changed, only the path was set).
- Like the original Grace QtGrace now properly accepts multiple commands in "evaluate expression". Just separate the commands with ';' or a new line. This did not work properly in some of the previous versions, because set-ids have been included incorrectely (this was intended for the undo-feature in conjunction with the commands-dialog).
- The undo-feature can now be actived or deactivated in the undo-dialog as well as in the preferences dialog.
- The settings for undo are now split into two different values. The one for the current setting and one for the setting on startup (preferences-dialog). This way you can set undo always active on startup and deactivate it temporarely without the problem of forgetting to activate it again for the next session.
- The setting for the general behavior now also includes the file-dialogs. If the behavior is set to QtGrace the file-dialog of the operating system is used directely whenever applicable. For data import this is not the case because to many options have to be set here. If you want the other behavior for the file dialogs while one kind of behavior is set (like Grace or like QtGrace) hold the Ctrl-key while selection the menu-action to open a dialog. For example: if the behavior is set to "like QtGrace" and you select "SaveAs" the file dialog of the operating system is opened. If you want the Grace-file dialog hold Crtl while clicking on File->SaveAs.
- The error messages on the command line if QtGrace is started with incomplete arguments have been altered. The messages now give more informations on what is missing. The intention was to be able to create a script for linux-bash-completion. (This script has been done by Francesco Sanfilippo.)
- When creating new sets in evaluate expression the new sets are no longer selected in the destination list.
- When creating new sets via "create new by formula" the new sets will be selected in the list they have been created in (i.e. where the popup menu has been summoned from).
- When switching between log-scaling and non-log-scaling for an axis the minor ticks are set to 1 for linear and 9 for everything else (because it is hard to find the optimal value for all settings).
- The "All Sets" or "All Graphs" item in a list is only displayed when there is more than one entry.
- The formula preprocessor has been rewritten. The preprocessor is used to make set-ids complete (like Y -> G0.S0.Y) and to replace set-ids. The new improved version now recognizes different commands in a single line (separated by ';') and also strings in a command ('"', but note, that the text inside '"' is condidered as a separate command and ids will also be replaced, only a '$' at the end of an id protects it against changes). If you use scripting a lot please recheck your script if you use more than one command per line or commands inside '"'.
- To display set point as letters you had to enter the ascii-code for the character in Grace and in previous version of QtGrace. Now I included a list with all visible characters instead (so you do not have to know the ascii-codes). I hope this is helpful.
- The size of every dialog is now stored in the ini-file when QtGrace is closed. When QtGrace is opened again the default dialog sized are used by default. But in the preferences there is now an option to use the last dialog sizes when opening a dialog. This is necessary if you own a monitor with very large resolution (like 4K). In this case QtGrace would usually be very little on the screen. To increase the size of QtGrace on screen you have to increase the font size, adjust every dialog size and increase the size of the buttons in the tool bar. Then you have to activate the option to use the last dialog sizes.
- I revised the code in many respects to prevent the huge amount of compiler warnings. I tried to reduce the warning to zero, but this was not possible for every platform (at least I did not find a solution that satisfies every compiler). Maybe I can improve this in the future.
- If there are several legends of several graphs present, then a double-click inside a legend box now opens the corresponding graph-appearance-dialog (for the correct graph-id, not only if the graph-id is the current graph-id). If multiple legend boxes are on top of each other, then the current graph-id is prefered (not the top-most legend).
- The popup dialog for the color selectors and linestyles can now be closed (without making changes) by pressing Esc or by clicking outside the popup (clicking outside did not work reliably so far, I hope it works now).
- The graphs viewing area can now be moved around with Crtl+Arrow-Keys (Up, Down, Left, Right).
- The "Del"-key now lets you delete sets or graphs from a list or lines from the spreadsheet-editor.
- If a button in the toolbar or in the Drawing-objects-dialog is pressed to do something the button should now stay pressed to indicate what action is currently active.
- In previous versions of QtGrace you could drag and drop ascii-files with datapoints into QtGrace to import the data. But QtGrace has been a bit picky about the line-endings. Now QtGrace accepts CR or LF or CR+LF as line-end-characters on every operating system.
- I included additional icons for some of the buttons. Some of them I have drawn myself (especially the icons in the drawing objects dialog). Suggestions for nicer pictures are welcome.
- In Grace as well as in previous versions of QtGrace the error-bars of set-points vanished when zooming in or changing the axis-limits and the set-point gets out of the viewing area. Now the error bars are still displayed even if the set-points are no longer visible. The underlying procedure is now a bit more demanding on the cpu, but I hope it is not too bad for anyone.
- I tried to include the possibility of using touch-gestures to control QtGrace. Unfortunately only zooming (two fingers on a touchpad together and then separate the fingers) works somewhat, but I would recommend against using it, because the result is usually not easy to control. If you still want to try it activate the option in Preferences->QtGrace-Behavior.
- The printing on physical printer has been put into two different places: In the main menu there is a Print-command (File->Print) and in the File Export Setup Dialog there is a Print-command in the menu as well. If the Print-command in the main menu is used a dialog will appear where the user can select the printer. All other settings are ignored by QtGrace. The project will always be printed full page (as far as it fits) with the printer-resolution set in the preferences (Preferences->Miscellaneous-tab). If you want more control over the printing use the Print-command in the menu in the File Export Setup Dialog. If you use this, then the resolution and the page size in pixels set in the dialog are used.


Bug-Fixes:
- The gracerc-file that has been user ba Grace for user-specific settings has been neglected in previous versions of QtGrace, because most settings are stored in the QtGrace_settings.ini-file. Now the gracerc-file is used after all settings have been taken from the ini-file. A few seldom used settings (like resolution of specific output drivers) can now be set in gracerc like it has been done in Grace.
- If QtGrace is opened with a filename (with non-absolute path) as command line option then the file was not always loaded correctely in previous versions. Now such files should be opened and the relative path to the working directory is to be used. I hope this works now as expected.
- The Esc-Button now properly halts all operations (like the right mouse-button). I used a wrong ascii-code for the Esc-key in previous versions.
- Sometimes the size-sliders (symbol-size/font-size) changed by themselfes. I hope I have finally removed this bug.


Special thanks to 
- Francesco Sanfilippo for lots of suggestions for improvements and helpful discussions
- Nimal Kailasanathan for additional work on the Server/Client extension
- Stefan Bernhardt for help with the creation of Linux-installation-packages


One further request (as always): If you find bugs or missing feature or if you have suggestions for improvements: Let me know immediately!
Thank you!

Warning: I changed some of the shortcut-keys in contrast to Grace: In order to use Crtl+Z for Undo I used Alt+Z for Zoom. In order to use Crtl+V for paste-operations I used Alt+V for viewport.
I hope this will be helpful to someone. Don't hesitate to report bugs/suggestions/wishes.


General Notes on QtGrace:

Dear User or Developer;

Thank you for your interest in my Qt-version of Grace!
In case you do not know already: Grace is a program to plot data and analyze it.
Grace is based on xmgrace / xmgr and uses the motif-framework for the graphical user interface.
The Grace-website is: http://plasma-gate.weizmann.ac.il/Grace/
QtGrace is based on grace-5.1.22 but uses the Qt-framework (formerly by Nokia, now Open-Source) as a graphical user interface instead (Qt and their respective logos were formerly trademarks of the Nokia Corporation but are now Open-Source as far as I know; see http://www.qt.io/).
Although I have tested QtGrace for quite a while now there may still be some problems / bugs present. Therefore give it a try, but be sure to read the Grace documentation (and save often) - and tell me when you find a bug so that I may remove it.

I hope this will be useful to someone else as well (I like my Qt-version of Grace :-) ).

My motivation: I had problems using Grace on different operating systems. Since I had written some applications using the Qt-framework (which can be found on http://www.qt.io/) I decided to modify the latest stable version of Grace (5.1.22) and replace all dialogs by equally looking Qt-replacements. I wanted to conserve the "look and feel" of Grace 5.1.22 because I wanted all Grace-users to be able to work with QtGrace immediately without having to adjust their workflow. Over the years I also included new features as I found them useful or after users suggested them.

I altered most of the source-files and therefore changes in Grace have to be implemented manually. Since there seems to be no major development around Grace-5.1.22 I believe that this approach is acceptable for now. All development on Grace seems to be focused on version 6 (or 5.99) which has a fundamentally different internal architecture. QtGrace is not compatible with Grace 5.99. There are other implementations of Grace 5.99 with newer graphic-libraries (like Qt and GTK) - just do a websearch if you want these.

This version 0.2.6 of QtGrace is capable of nearly all of the operations Grace 5.1.22 is able to do. Some minor functions are missing or work only on some platforms (see below). I have inserted extra functions and interface additions I found helpful (see help-page on QtGrace-Extensions). There are a few minor known problems (see below).

The internal procedures are the ones from Grace 5.1.22 (internal data management, calculations and so on). I have only recreated the user interface in Qt which makes it possible to compile QtGrace natively (!) on Windows, Mac OS X and Linux. You can load your old Grace-project files (usual extension: '.gar' / '.xgr'). With Qt it is also possible to easily translate the user interface and make the language of the interface selectable (I have made an attempt for a test-translation to German. The file is included but very far (!!) from being complete. If there is someone out there willing to complete the german translation or generate a translation to any other language feel free to do so using the "Linguist" program included in the Qt-installation. If you send me the translation I will include it into the downloadable zip-file.).

Don't alter the structure or remove content from the folders of QtGrace. The executable relies on the relative positions of the files it needs. Just use symbolic links to start QtGrace from your desktop.

I am always interested in hearing your opinion on QtGrace or in your bug-reports. Suggestions for improvements are welcome as well!
To contact me please use email: andreas.f.winter@web.de

I hope you enjoy QtGrace!

Best regards
Andreas Winter (andwin)


Copyright and License-Terms:

Copyright (C) 2008-2017 by Andreas Winter
andreas.f.winter@web.de
QtGraceTcpServer- and -Client-code (C) 2014-2017 by by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

libharu and fftw3 are included unmodified to be easily accessible. See QTGRACE_EXTENSIONS.html for further informations and copyright notice.
For the precompiled releases for Windows and MacOSX I include dlls and frameworks that are part of Qt. These files are under the same copyright as the Qt-library. I included the files for convenience unmodified.

How to build QtGrace:

QtGrace is based on the Qt-Framework which has to be obtained via http://www.qt.io/.
Please use at least version 4.6 of Qt to compile QtGrace (Qt5.5 or higher, is recommended, especially for QTcpSocket, please do not use 5.0 because it seems problematic)!
Please install the Qt-SDK which includes the Qt-Creator IDE. If you are an experienced developer, you may also be able to compile QtGrace without the Qt-Creator, but I will not describe how to do so here.
Since you have extracted this folder you may notice, that there are some sub-folders. It is important for QtGrace to keep the structure of this folder in order to find some files at runtime. I have also included the original source-code-zip-file from grace-5.1.22 as a tar-archive. You will not have to extract it. This is included just for completeness and for developers use in case someone needs it. 
I assume in the following part, that you have installed Qt and you have got a fully functional Qt-Creator. 
To compile QtGrace: 
First step: Open Qt-Creator and do File->"Open File or Project..." (you may have to specify the file format as "Qt Project file (*.pro)"). Select the file "src.pro" in the 'src'-sub-folder. On the left hand side of the IDE-window you should now be able to see all source-files that are part of QtGrace. (On newer version of QtCreator you may be asked to specify the target to compile QtGrace for - use the 'Desktop'-version in this case.)
Second step: At the left hand side of the Qt-Creator-window you find some icons. Click on the "Projects"-icon and make sure a suitable path for the Qt-Installation is set at the general section. (You may have to consult the Qt-Manual on some systems on how to do so correctly. Especially in newer versions of QtCreator you have to prepare the compiler settings in a special way, so please consult the Qt-documentation.) You have two options now: either set the build-path in the general build settings to the 'bin'-subfolder or remember the build path that has been set automatically and copy the executable after the compilation to the 'bin'-subfolder. You will also have to set the destination of the executable to be executed from within Qt Creator in the settings in order to run QtGrace after the compilation from within Qt Creator. On newer version of the QtCreator you have to manage the different kits (see the Qt-documentation).
Third step: Click the 'Edit'-icon on the left side. Click on the 'Build All'-Icon (the one with the hammer in the lower left hand corner). This will initiate the process of compilation and may take a while. You may also see a few thousand warnings which can be ignored (hopefully) but there should be no errors (if you get errors, report them to me and I will provide information on how to solve this problem). (Special platform specific notes: see below)
Fourth step: Everything has been compiled now and you should be able to start QtGrace. Either you double-click on the qtgrace icon (or qtgrace.exe) in the bin-folder or you set the execution path in the project settings to this file and run it from within the Qt Creator.
Done! 

On Windows:
You have to set the path to the Qt-dlls in your PATH-variable or copy the dlls to the bin-folder (if you have a complete Qt-installation you can run the 'windeployqt'-tool -- see the Qt-documentation for how to do so)

On Linux:
If you continue getting messages like: invalid viewport coordinates --> please contact me.

On Mac OS X:
Executables are organized as disguised folders on such systems. Therefore after compilation in order to work in a MacOS-way you have to open the package-contents of qtgrace (right-click on qtgrace in the 'bin'-folder and select 'show package contents'). Then go to 'Contents'. Copy the contents of the original unzipped folder except the 'bin'-subfolder and the grace-5.1.22.tar.gz into this 'Contents-folder'. Copy the contents (except 'qtgrace') of the 'bin'-subfolder into the 'MacOS'-folder. Now you should be able to execute qtgrace by clicking on qtgrace in the 'bin'-subfolder. It should also be possible to move/copy QtGrace in every folder you like just like any other MacOS-application (put it for example in your Applications-folder). There is also a deployment-tool: 'macdeployqt' (see Qt-documentation).


Activate FFTW3- and libHaru(PDF)-support:
See QTGRACE_EXTENSIONS.html for further informations, go to Preferences-->Micellaneous and see whether the libraries are usable and activated.
If the libraries are not compiled into QtGrace statically you have to tell QtGrace where the dynamic library is located. This should usually be the bin-subfolder in the QtGrace-directory. The name of the library is usually libfftw3-3.dll and libhpdf.dll. If everything is ok you should see green instead of red boxes on the right hand side of the dialog.
If the Haru-PDF-library is activated you have to use the "PDF(Haru)"-option as the output format in the Export Setup Dialog to export pdf-files. Do only use the PDF-output-format (without "(Haru)") if the Haru-pdf-library is not available or if you have to use the QtFonts. 

Known Problems:

- Printing directly to a printer via the native printer-dialog is possible but I have not tested it extensively (but it seem to work in an acceptable way). Most settings are not mirrored in the native dialog. Direct printing without the native printer-dialog is only possible on systems that support the 'lpr' printing command (or any other printer-command like 'kprinter' on some linux-systems) and accept postscript files (i.e. a system command like 'lpr file.ps' will actually print the contents of 'file.ps' on the standard-(line)-printer). Printing to files is always possible. You have to physically print the file-contents via a different/external program (like OpenOffice/LibreOffice). You may also copy the whole page in the clipboard via View->PageSetup and paste it into your external program. Printing on Mac-system without a physical printer present can be tricky if you use Qt5 - sorry (previously the pdf-option was always accessible)
- On some Linux-systems the scrollbars are not displayed correctly but they still stay usable (I don't know why, yet. Seems to be a Qt-problem...; I also don't know whether this bug is still present, since I have not seen it in a while).
- Pipes not working on Windows.
- The switching between Grace-fonts and QtFonts is not advisable. It is possible in most cases, but especially for the Symbol-font problems are to be expected - sorry. This has several reasons especially with the ordering of characters in the T1-Symbol-font in Grace and UTF-8-fonts. At the moment I do not know how to solve this problem. I made an attempt by making the symbol-font special (a separate setting in the preferences). This should help - but this approach may not be flawless.
- Loading files (especially old ones) relies heavily on the encoding set in the Preferences-dialog (QtGrace-Behavior). For loading example-files it is often advisable to use the Latin1-encoding (or something similar). Newer files should contain the encoding used during saving and should be loadable without problems.


Things that work in Grace, but not in QtGrace:

- I have rewritten the spreadsheet-editor. Therefore it may not work as you are used to it (or maybe you won't even notice).
- I have written a simple text-editor for creating and editing sets. On Unix the default in Grace is to open 'vi' ('Create new->In text editor'). This option is not portable (at least not in a way I would like). I used an internal text-editor because this seems to be comfortable enough (suggestions for improvements are welcome).
- The pipes may work a bit differently in QtGrace, because I could not include the 'pipe-contents-changed' signal into the main-application loop like Grace does. Pipes also do not work on Windows, yet.


Additions to QtGrace (not present in Grace): see help-page on QtGrace-Extensions (because there are a lot)


Pipes (I include this here and not in the help-file, because I do not want to advertise this feature due to it's incompleteness):

The use of named pipes via the '-npipe' option or starting QtGrace in a pipe via '-pipe' or opening a pipe from Data->Import->Ascii is supported now. The drawback at the moment is: This does not work on Windows, yet. The usage of pipes is a bit different on Windows and I have to find out how to reliably use them there, sorry. Any helpful advice is welcome here! 
For the pipes (also called "Real time input") I included a new dialog. In 'Window->Monitor real time inputs...' you can see which pipes are currently monitored for input. You can also activate or deactivate the monitoring here via a checkbox. Internally in order not to block the other operations of QtGrace the input via pipes is not real time but in specified intervals. The time between updates of the real time input can be set here. I set it to 1 second by default. You can also open and close pipes via this dialog. I hope this is helpful for some people.

Any suggestions for improvements, any bug reports or other comments are welcome.
Thank you.


Version history:
v0.1.2		initial release on sourceforge.net
-		I made a few silent updates to remove some more or less minor bugs
v0.1.2a		bug-fix-release: major bug in "Arrange graphs"-dialog removed; adjustments on the sliders (especially their resize-behavior); changes in the routine for loading gracerc.user: this file is searched first in the working directory, then in the directory of the executable and last in the users home directory; special-tick-marks-problems solved
v0.1.3		bug-fixes: the transformation between word- and viewport-coordinates has been included in the objects-dialogs (boxes, ellipses ...); some of the dialogs have text-lines in which usually numbers are entered (like the start- and stop-values in the axis-dialog), now more of them accept parametric values as well (like the constant PI), this is a feature of Grace that has been temporarily deactivated during the development phase of QtGrace and I forgot to reactivate it, this has now been done; the native-printer-dialog now uses the set page-orientation as a default; 
		Undo/Redo included (partially and experimental)
v0.1.3a		hotfix for a severe memory-bug in the undo-function; undo is still unfinished and experimental
v0.1.4		first release with complete Undo/Redo; several bug-fixes of mostly minor stuff
v0.1.5		several bug fixes and improvements on the repaint-system (which is especially necessary for newer Qt-versions)
v0.1.6		some bug fixes; addition of immediate updates and support for simple LaTeX-type text-input
v0.1.7		minor bug fixes; addition of copy/paste and drag/drop; minor improvements on automatic script generation
v0.1.8		bug fixes; several improvements; addition of color management; filling the area between sets; rotation of boxes and ellipses
v0.1.9		bug fixes(some severe ones); transparency added for printing in files; spread-sheet editor rewritten
v0.2		Page-zoom, selecting multiple graphs for zooming, reactivating pipes (non-Windows-systems)
v0.2.1		Support for csv-import
v0.2.2		'-free' option and 'immediate update' reactivated, experimental support for non-latin-characters and different fonts; support for multiple files in binary import and the use of a trigger channel ; bug fixes
v0.2.3		minor bugfixes; print-command saved in preferences; selection of odd or even set-ids; addition of fit-button for page-zoom-slider; minor changes to improve UTF-8-support in file-paths
v0.2.4		several bugfixes (major and minor); further improvement in UTF-8-support (very extensive internal changes); made decimal-separator selectable throughout the whole QtGrace-Gui; improvement in the script-generation (execution of scripts with automatically replaced set-ids); slider-value made editable; QtGrace-help-page; QtGrace-examples
v0.2.5		rework of the binary import (incl. drag-and-drop); addition of regions-master-dialog; additions to the commands-dialog to extend the scripting possibilities; increased the number of fit-parameters to 20; redesign of the output-dialog, separation of file-output from physical printer, adding a quick-selection-options to set the resolution; added the possibility to use libHaru (for pdf-output) and libFFTW3 (for better fourier-transformation); further extension of the preferences dialog (many things are now user-adjustable); drag-and-drop for file-import-selection-lines; more comfortable dragging-and-dropping of Grace-project-files (agr); separate dialog to let you extract single sets from an agr-file; restrict sets to regions and swap columns of a set (like swap X- and Y-axis); extension of the feature-extraction-dialog (more features); project description now visible in the open-projects-dialog and editable in the plot-appearance-dialog; addition of a file-path-stamp to the project; global font-size-multiplier; further options for placing the legend-box (manual and automatic alignment with the graphs borders); changed the apply-to-option for the axis-dialog; additional dialog to determine set-intersection-points and -angles; addition of QTcpSocket-support for remote control (by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB); bug-fixes
v0.2.5a		Bugfixes
v0.2.6		Lots of interface improvements: Setup-Wizard, Apply single settings on multiple sets, text editable in main drawing area, Icons in set lists, Highlighting, Extended context menu in main drawing area, Transparency, Pan and Move, Page-size-change by click and drag, Error-bars in legends, connected errorbars, Ignore a set during autoscaling, Sync.selection of source- and destination, Size of tool bar changeable, New polar plot, Mouse-wheel-zooming and -size-changes, title- and subtitle-position adjustable, New viewport stack control, Antialiasing for all objects (not only text), Auto "Pack all sets", More icons on buttons/in lists, Error-bars concidered in autoscale, Popup-menu cleared up and extended with sub-menus, Multiple commands in "evaluate expression" allowed now (again), Undo setting separated in current- and startup-setting, More explanations and unified messages on incomplete command line arguments, Auto selection of new sets changed, "All Sets"/"All Graphs" only displayed if more than one entry in list, New formula preprocessor, Letters as symbols for set-points can now be selected from a list (no ascii-code needed any more), Dialog sizes stored in ini-file, Select legend by double-click, Popup for the color selectors and linestyles can be closed by pressing Esc or by clicking outside, Shift viewing area with Ctrl+Arrow keys, delete sets or graphs or lines in spreadsheet with Del-key, Tool-buttons stay pressed as long as the action is active, Import data with drag and drog with CR / LF / CR+LF as line-end-characters, Additional icons, Error-bars now visible even if set point outside viewing area, Minimal support for touch-gestures, gracerc-file fully usable again, improved support for relative paths for files, Esc-key halts all operations (as intended), slider-values no longer unstable (no shifting when apply is pressed, no more autonomous movements), Code revision.

Have fun using QtGrace!

Best regards
Andreas Winter (andwin)
 
