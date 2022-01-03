# sloppycounter.c

This program intends to show a method of updating a global counter using local counters of multiple threads shown in the book 'Operating Systems' by Remzi H. & Andrea C. Arpaci-Dusseau.
Proofing that the GC values accuracy and the overhead time are dependent on the chosen threshold value S. Conlcuding that large thresholds are useful for systems dependent on fast response and whereas small thresholds for accurate dependent systems.
A large threshold leads to small OVH but inaccurete GC value and vice versa. 

## Code ran and outpu example: ./sloppycounter.c
> Value of global counter with threshold[2]:		       10000 \
> Time measured in nano seconds to finish thread[1]:	 383102 \\

> Value of global counter with threshold[4]:		       10000 \
> Time measured in nano seconds to finish thread[2]:	 261135 \\

> Value of global counter with threshold[8]:		       10000 \
> Time measured in nano seconds to finish thread[3]:	 532834 \\

> Value of global counter with threshold[16]:		       10000 \
> Time measured in nano seconds to finish thread[4]:	 537250 \\

> Value of global counter with threshold[32]:		       9984 \
> Time measured in nano seconds to finish thread[5]:	 600587 \\

> Value of global counter with threshold[64]:		       9984 \
> Time measured in nano seconds to finish thread[6]:	 615126 \\

> Value of global counter with threshold[128]:		     9984 \
> Time measured in nano seconds to finish thread[7]:	 679913 \\

> Value of global counter with threshold[265]:		     9805 \
> Time measured in nano seconds to finish thread[8]:	 707932 \\

> Value of global counter with threshold[512]:		     9728 \
> Time measured in nano seconds to finish thread[9]:	 731569 \\

> Value of global counter with threshold[1024]:		     9216 \
> Time measured in nano seconds to finish thread[10]:	 757484 \\

