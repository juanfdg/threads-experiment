LDFLAGS=-lpthread

all: lab3_threads lab3_sequential entry.txt

entry.txt: create_entry.py
	python $<

clean:
	$(RM) lab3_threads lab3_sequential entry.txt
