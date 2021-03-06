#ifndef ATOM_INCLUDED
#define ATOM_INCLUDED
extern		  int   Atom_length(const char *str);
extern	const char *Atom_new (const char *str, int len);
extern  const char *Atom_add(const char *str, int len);
extern  const char *Atom_string(const char *str);
extern	const char *Atom_int(long n);
extern  	  void  Atom_free(const char *str);   
extern        void  Atom_reset();
#endif
