/* ############################################################

                    ___           ___           ___     
        ___        /\__\         /\  \         /\  \    
       /\  \      /::|  |       /::\  \       /::\  \   
       \:\  \    /:|:|  |      /:/\:\  \     /:/\ \  \  
       /::\__\  /:/|:|  |__   /::\~\:\  \   _\:\~\ \  \ 
    __/:/\/__/ /:/ |:| /\__\ /:/\:\ \:\__\ /\ \:\ \ \__\
   /\/:/  /    \/__|:|/:/  / \:\~\:\ \/__/ \:\ \:\ \/__/
   \::/__/         |:/:/  /   \:\ \:\__\    \:\ \:\__\  
    \:\__\         |::/  /     \:\ \/__/     \:\/:/  /  
     \/__/         /:/  /       \:\__\        \::/  /   
                   \/__/         \/__/         \/__/    


    File:          util.h
    

    Purpose:       util module with generic
                   functions in it, which
                   can be used in any given
                   project.
    

    Remarks:       - consult man pages for
                     explanation of function
                     details.
    
    
    Author:        P. Leibundgut <leiu@zhaw.ch>
    
    Date:          11/2015

   
   ############################################################ */

#ifndef UTIL_H_
#define UTIL_H_

#define SECOND_IN_NANOS ( 1000L * 1000L * 1000L )

/* public function prototypes */
void fail_whale( FILE* out );
long try_strtol( char* str );
void timespec_sub( struct timespec* a, struct timespec* b, struct timespec* res );

#endif /* UTIL_H_ */
