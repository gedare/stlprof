stlprof
=======

GNU Standard C++ Library v3 STL Profiling for gcc 4.7.1

This software has whatever license the libstdc++ had at the time of writing.

Replace gcc_4_7_1_release/libstdc++-v3/include with include.

Most of the changes are localized to the include/profile/impl directory
and would be a good place to start if updating to newer versions of GCC.

This software is incompletely tested and not production quality. The timing
mechanisms are hard-coded for x86 (ia32) only with the rdstc built-in.
The timing mechanism does not consider variable effects such as interrupts and
context switches.

The original profiling framework is described in
* Lixia Liu and Silvius Rus. "Perflint: A context sensitive performance advisor
  for C++ programs." In Proceedings of the 7th annual IEEE/ACM International
  Symposium on Code Generation and Optimization, CGO ’09, page 265-274,
  Washington, DC, USA, 2009. IEEE Computer Society

Additional information about the modifications here can be found at
* http://gedare-csphd.blogspot.com/2013/01/profiling-c-applications-class.html
* Gedare Bloom. "Operating System Support for Shared Hardware Data Structures",
  Dissertation Thesis, The George Washington University, 2012.
  http://home.gwu.edu/~gedare/pdf/BloomThesis.pdf

