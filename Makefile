LDFLAGS=-lpthread

all: lab3_threads lab3_threads_wrong lab3_threads_no_debug lab3_sequential entry.txt

entry.txt: create_entry.py
	python $<

clean:
	$(RM) lab3_threads lab3_threads_wrong lab3_threads_no_debug lab3_sequential
	$(RM) entry.txt threads.out threads_no_debug.out threads_wrong.out sequential.out
