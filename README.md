# GERNI: Grammatical Evolution Regulatory Network Inferrer

This was my term project for my Bioinformatics Algorithms class. There's an abundance of time-course DNA microarray data that can help analyze the interactions among genes in an organism. These interactions can be modeled using gene regulatory networks (GRNs). Ordinary differential equations are a popular way of describing GRNs. The goal of my project was to develop a program to obtain a set of first-order ordinary differential equations to describe a given data set. This is similar to the symbolic regression problem.

Please refer to https://www.cs.colostate.edu/~andrescj/proj/gerni/ for more details.

## Compilation

1. Download Boost (http://www.boost.org/). GERNI was tested with version 1.60.0. Extract to some folder. I will refer to this folder as:

   /path/to/boost

   No need to compile/install anything.

2. Download AGE (http://nohejl.name/age/). GERNI requires AGE version 1.1.1. I had to add `#include <string.h>` to src/AppTimeTabling.cpp because it was complaining about memset and memcpy not being declared. I also had to add Chromosome.h to the INCLUDES variable in the Makefile. Otherwise, it won't install it.

   Install to some folder using (adapted from AGE's instructions):

   `make INSTALL_PREFIX=/path/to/age install`

3. Download ExprTk (http://partow.net/programming/exprtk/index.html). I couldn't find version information. Extract to some folder. I will refer to this folder as:

   /path/to/exprtk

   No need to compile/install anything.

4. Download PLplot (http://plplot.sourceforge.net). GERNI was tested with version 5.11.1.

   In my system (Fedora 23, x86_64), I had to run (before compiling):

   `export CXX="g++ -fPIC"`

   Otherwise, it would give me a "relocation can not be used" error.
   
   Install to some folder using (adapted from PLplot's instructions):

   ```
   mkdir build_dir
   cd build_dir
   cmake -DCMAKE_INSTALL_PREFIX=/path/to/plplot ../
   make
   make install
   ```
   
5. Modify GERNI's Makefile to change the library installation paths:

   ```
   BOOST = /path/to/boost
   AGE = /path/to/age
   EXPRTK = /path/to/exprtk
   PLPLOT = /path/to/plplot
   ```

6. Compile:

   `make`

7. Run example:

   `./GERNI oscillating_spring.txt`
   
   The Bash script ./GERNI wraps the GERNI4CPP executable to provide default parameters. These parameters are easy to change in the script. The file passed as an argument represents the target values and has the following format:
   
   ```
   t1 t2 t3 t4 ... tN
   y11 y12 y13 y14 ... y1N
   y21 y22 y23 y24 ... y2N
   .
   .
   .
   yD1 yD2 yD3 yD4 ... yDN
   ```
   
   Where `t1 ... tN` are the values of the independent variable. Each line thereafter are the values for each function at each value of the independent variable. GERNI will output a system of D differential equations. Each equation has the form `dy[i] := <expr>`. See ODESystemFitnessEvaluator.cpp for details of the grammar that defines `<expr>`.
   
   The oscillating_spring.txt sample data was generated from the following system:
   
   ```
   dy[0] := y[1]
   dy[1] := -1.5 * y[0]
   ```
   
   Where y[0] is the how much the spring is compressed/streched and y[1] is the first derivative. This system is simply the first-order version of the second-order equation for oscillating springs:
   
   (From www.stewartcalculus.com/data/.../upfiles/3c3-AppsOf2ndOrders_Stu.pdf)
   
   ```
       dx
   m * -- = -k * x
       dt
   ```
   
   Where k = 3 and m = 2. You can also run
   
   `./GERNI oscillating_spring.txt plot`
   
   This will plot the resulting ODE system and the target data for comparison. The obtained model is shown in dashed lines and the target data is shown in solid lines. The colors are assigned as follows:
   
   ```
   y[0]:  RED,
   y[1]:  YELLOW,
   y[2]:  GREEN,
   y[3]:  AQUAMARINE,
   y[4]:  PINK,
   y[5]:  WHEAT,
   y[6]:  GREY,
   y[7]:  BROWN,
   y[8]:  BLUE,
   y[9]:  BLUE_VIOLET,
   y[10]: CYAN,
   y[11]: TURQUOISE,
   y[12]: MAGENTA,
   y[13]: SALMON,
   y[14]: WHITE
   ```
   
   Warning: don't attempt to plot when D > 15.
