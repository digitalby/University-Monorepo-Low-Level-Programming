# University Monorepo: Low Level Programming

There are fourteen assignments. The goal of each is to help learn about various aspects of system-level programming by developing applications in feature-constrained environments. 

Extra credit was given for implementations written using tools and programming languages on the lowest possible level.

---

List of assigments:

1. **File IO**

   *Input a file extension. Look for files that have that extension, then output their names to a text file.*

2. **Filesystems**

   *In a FAT/NTFS file system, input a cluster number, then output the name of the file for which that cluster was allocated.*

3. **RAM allocation**

   *Implement a data structure (array, linked list, BST) with basic operations.*

4. **Interrupts**

   *Develop a memory resident program that captures the keyboard interrupt and messes with its input.*

5. **Memory resident software**

   *Develop a key logger. It should output captured keystrokes every couple of seconds to a text file.*

6. **Win32 API basics**

   *Develop a GUI application with a custom icon and a custom cursor that has a moving label control.* 
   
   *(For this and the following assigment, it is allowed to use any other GUI framework, e.g. Qt.)*

7. **GUI controls**

   *Develop a GUI application with two `ListBox`es, and `Button`s responsible for their create/delete/move operations.*

8. **GUI static graphics**

   *Develop a GUI application with owner-drawn `Button`s, and a canvas. On the canvas, draw an image using primitives.*

9. **GUI dynamic graphics**

   *Develop a GUI application with an image sprite that moves to a requested position, while animated.*

10. **Window messages**

   *Develop a GUI application with two windows, where one sends custom window messages, and the other receives and processes them as requested.*

11. **Multithreading**

   *Every two seconds, generate a random number and display it in an `Edit` control. There are three `Edit` controls to process, all having their own number.*

12. **Race conditions**

   *In an `Edit` control, display the text "First", "Second", "Third" in this exact order with the same delay between every update.*

13. **Windows process priorities**

  *Develop a GUI application which can display currently running processes, and modules of a specific process. It should also be possible to change the priority of any process.*

14. **Windows registry**

   *Display the `HKEY_CURRENT_CONFIG` registry hive on-screen.*