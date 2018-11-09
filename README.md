# pubsubtask

Slapped together in the wee hours of the night for YOU.

Typical cmake scenario:
	1) in the sort directory mkdir 'build'
	2) cd to that build directory
	3) issue 'cmake ..'
	4) 'make'
	5) run 'create_data' with a file name argument, this will create the data to sort
	6) run 'pubsubsort with: the file created in step 1, a name for the output file,
		and an algorithm, your choices are `mer' (merge sort), and `bub' (bubble sort).
