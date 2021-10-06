<h1> parallel-processing-assignments <h1>
# PP assignments made in C language using MPI and openMP
# If you're linux user you can simply use the below commands on your terminal. But, in case you're a windows user you need first to install oracle VM VirtualBox.
# after installing and setting it up. starting from login
username: root and password: Hpc@fc1
# choose the directory you want to put codes in in the VM.
>>mkdir codes <br />
>>chmod 777 codes/ <br />
# link the directory on your physical device with the directory on the VM.
>>mount -t vboxsf shared codes/ <br />
>>cd codes<br />
>>ls <br />
files in your windows folder should appear if any exists
>>mpd & <br />
# how to run the file.
>>mpicc -o greetings.out greetings.c <br />
>>mpiexec -n 4 ./greetings.out <br />


# How to run openMP
gcc -fopenmp -o out.o lab6.c <br />
export OMP_NUM_THREADS=4 <br />
./out.o
