/*
* from https://wafiharowa.wordpress.com/2013/11/03/tutorial-5-part-5-bitmap-font/
*/
 // Implement Char-to-Frame mapping.
 int getCharFrame(char c) {

  // ASCII code for 0-9
  if((c >= 48) && (c <=57)){

   // 0-9 are located at Frame 16-25
   return (c - 48) + 16;

  // ASCII code for A-Z
  } else if((c >= 65) && (c <=90)){

   // A-Z are located at Frame 33-58
   return (c - 65) + 33;

  // ASCII code for a-z
  } else if((c >= 97) && (c <= 122)){

   // a-z are located at Frame 65-90
   return (c - 97) + 65;
  }

  // Everything else
  return 0;
}