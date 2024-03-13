/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./output.h"
#include "./rand64-sw.h"
#include "./rand64-hw.h"
#include "./options.h"

/* Main program, which outputs N bytes of random data.  */
int main (int argc, char **argv)
{
  struct options options;
  parse_option(argc, argv, &options);

  // first check that options are valid
  if (!options.valid) {
    fprintf(stderr, "Invalid options statement.")
    return 1;
  }
  long long num_output_bytes = options.num_output_bytes;
  
  /* set up function pointers that can be either the hardware/software functions */
  void (*initialize) (void);
  unsigned long long (*rand64) ();
  void (*finalize) (void);

  /* look at options and decide how to operate */
  if (options.input == NONE) {
    /* spec says to use RDRAND as the default and fail if hardware rand is not available */
    if (rdrand_supported()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
    else {
      fprintf(stderr, "No option supplied for input -> rdrand is default -> rdrand not supported.");
      return 1;
    }
  }
  else if (options.input == RDRAND) {
    /* same code but different error */
    if (rdrand_supported()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
    else {
      fprintf(stderr, "rdrand not supported.");
      return 1;
    }
  }
  else if (options.input == LRAND48_R) {
    initialize = hardware_rand64_init;
    rand64 = hardware_lrand48;
    finalize = hardware_rand64_fini;
  }
  else if (options.input == FILE) {
    read_file(options.rand_src_filename);
    initialize = software_rand64_init;
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  }
  else {
    fprintf(stderr, "Input not detected as invalid, but could not determine randomness function.");
    return 1;
  }

  initialize();
  int output_word_size = sizeof rand64();
  int output_errno = 0;

  if (options.output == STDOUT) {
    do {
      unsigned long long computed_rand = rand64();
      int output_bytes;
      if (num_output_bytes < output_word_size) {
        output_bytes = num_output_bytes;
      }
      else {
        output_bytes = output_word_size;
      }
      if (!writebytes(computed_rand, output_bytes)) {
        output_errno = errno;
        break;
      }
      num_output_bytes -= output_bytes;
    }
    while (0 < num_output_bytes);

    if (fclose(stdout) != 0) {
      
    }
  }
  /*
  // Check arguments.
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  // If there's no work to do, don't worry about which library to use. 
  if (nbytes == 0)
    return 0;

  // Now that we know we have work to do, arrange to use the
   //  appropriate library. 
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  if (rdrand_supported ())
    {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
  else
    {
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    }

  initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
  */
}