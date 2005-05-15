### Read two Adobe Font Metric files, and compute tables of the
### differences in character positions
###
### Usage:
###	awk -f afmdiff.awk afm_from_type1 afm_from_ttf
###

/^C /		{
		    name = $8
		    if (name in C1)
		    	C2[name] = $2
		    else
		    	C1[name] = $2
		}


END		{
		    show_index_diffs(C1, C2)
		}

function show_index_diffs(array1, array2, name)
{
  for (name in array1)
    {
      if (!(name in array2))
	printf("/* 0x%02x */ 0x??, // %s\n", array1[name], name)
      else
	printf("/* 0x%02x */ 0x%02x, // %s\n", array1[name], array2[name], name)
    }
}

