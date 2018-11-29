///////////////////////////////////////////////////////////////////////
//
//  Creazione e visualizzazione di una sfera con indici della
//  texture
//
///////////////////////////////////////////////////////////////////////

#if !defined(SFERA_H_INCLUSA)
#define SFERA_H_INCLUSA

//  Creare una sfera chiamando
void CreateSphere( float radious, int sections );


//  Disegnare (solo parte fra glBegin e glEnd) con
void DrawSphere(float r);

#endif    // SFERA_H_INCLUSA
