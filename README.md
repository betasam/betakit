bkit
====

Copyright (c) 2005-2011 Sunil Beta Baskar <betasam@gmail.com>
This utility is licensed for usage under a BSD style license.

Please Read the DISCLAIMER before using this utility.


INTRODUCTION
------------
bkit is meant to be an embedded software developer's toolbox. I
have been trying to find a suitable replacement for busybox for
embedded linux distributions to provide more choice. The sub-modules
of bkit intend to be usable individually. This is to provide more
usability as anyone would see fit by making sure its easy to rip
something from the main scaffold and use it individually.

bkit will use dpkg as the basic packaging system to help resolve
dependencies better for targets. The emdebian project does exist and
this code could one day be contributed to emdebian. We can also borrow
packages for various targets supported currently by emdebian. This
would be specific to the Distribution Building kit.


GOALS
-----
bkit intends to provide the following
1. A Userspace library to simplify embedded application development
2. A Kernel toolkit to simply driver writing for embedded devices
3. A Distribution building kit for Embedded Targets which will be
customizable with minimal effort for any specific target.

1. The Userspace Library
------------------------
The Userspace library in turn provides
    a) System Functions
       - for memory management
       - for system profiling
    b) Data Structure Abstractions
    c) User Interface Primitives
    d) A Test Scaffold for all of the above

2. The Kernel Toolkit
---------------------
The Kernel toolkit will provide
    a) Device Driver Templates
       - for Framebuffer devices
       - for i2c based I/O control
       - for 1-wire based control
       - to provide firmware support
       - for HID/event driver models
    b) Profiling and Instrumenting Templates,
    c) Kernel Tree independent Makefile templates.
    d) LTP derived testsuite to qualify a linux kernel.

3. The Distribution Building Kit
--------------------------------
The Embedded Distro kit will provide
    a) Cross Compiler Toolchains for
       - ARMv4, ARMv5, MIPS32, xScale ISAs
    b) Cross Compiler Script 
       - to build custom cross-compilers
    c) Packages for use on the Dev-station
    d) Packages for use on the Target-Platform
    e) Filesystem building kits supporting
       ext2, ext3, jffs2, yaffs, yaffs2
    f) An extensible bootloader architecture
    g) Target Deployment tools
    h) CML2 based configuration environment



PLANS
-----
With a long list of goals which might take a long time and help from
several contributors, there are some ambitious plans to add to the
Distribution build kit
	     - A Platform/Machine Simulator
	     - Target Deployment GUI
	     - Multiple JTAG/ICE/Serial/Ethernet Transport Support


WHY?
----
Existing solutions like ptxdist have specific designs that create a
tightly coupled environment. This has been quite annoying when you
wanted to pull out things by hacking through Makefile
structures. bkit intends to allow the user to remove an entire
block or component and replace it with the user's version with ease.

Most projects like crosstool and other toolchain building scripts from
emdebian require installation of a full kit. bkit intends to be
usable as a whole package or its parts.

OpenEmbedded has several packages. However, it is tied down to bitbake
and requires some work to remove that dependency.


NAME
----
Why "bkit"? Originally the initiative was a small pet-project of mine
to be called Betakit. However, later the goals were revised to add so
many features that a lone developer would find difficult to deliver to
the community. So, a few friends came up with something really simple,
"Embedded Kit". After reviewing exactly what we wanted people to do,
we wanted to have a unixish name that would mean something close to
bake it. Hence to avoid all the naming confusion, the unix name bkit
was chosen.

LICENSE
-------
This entire kit and all its subcomponents are licensed under the BSD
License. The idea is to promote open-source developers to come and
contribute simultaneously allowing companies to deploy it without
worrying about the licensing.

DISCLAIMER
----------
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

QUESTIONS
---------
If you have a specific question regarding bkit or are in search of
a feature write to "Sunil Baskar" <betasam@gmail.com>.
Subject: [bkit] <descriptive-subject>
