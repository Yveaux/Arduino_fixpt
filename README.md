Arduino_fixpt
=============

Fixed point (16.16 & 8.8) library for Arduino to replace resource hungry float/double.
Implementation based on libfixmath (https://code.google.com/p/libfixmath).
See also http://en.wikipedia.org/wiki/Libfixmath.

Works on AVR (Uno, etc) and ARM (Due, etc) based Arduinos.

PS: The examples may not compile, only Fix16_benchmark was tested and fixed from the original repository.

Benchmarks (Fix16_benchmark.ino example)
```
Arduino Due fix16_t vs double Benchmark (time in micro seconds):
Op      double	  fixpt	    speed improvement fixpt over double
Mult    372545	  97631	    381.58%
Div     1335860   203392	656.79%
Add     457899	  14830	    3087.65%
Sub     456773	  37275	    1225.41%
Sqrt    14453     2842	    508.55%
```

```
Arduino Due fix16_t vs float Benchmark (time in micro seconds):
Op      float	    fixpt	speed improvement fixpt over float
Mult    304898	  103902	293.45%
Div     640075	  206859	309.43%
Add     356009	  22301	    1596.38%
Sub     361209	  39842	    906.60%
Sqrt    14750	    2662	 554.09%
```
Released under MIT License.
