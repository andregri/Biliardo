///////////////////////////////////////////////////////////////////////
//
//  Creazione e visualizzazione di una sfera con indici della
//  texture
//
///////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <math.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library

//  Senza questo l'include produce una valanga di warning
#pragma warning(disable: 4786)

#include <vector>

#include "Sfera.h"

///////////////////////////////////////////////////////////////////////
//  Punto o Vettore 3D in coordinate omogenee.
//  I vettori hanno w = 0, i punti w = 1
//  float o double (o altro sensato...) a scelta.
template <class T> class P3d
{
public:
  T x,y,z,w;

  P3d() {  x = y = z = 0; w = 1; }  // non cambiare, il programma si
                                    // fida che inizializzi a 0 !
	P3d(T xx, T yy, T zz, T ww) {
		x = xx; y = yy; z = zz; w = ww;
	}
	P3d(T xx, T yy, T zz) {
		x = xx; y = yy; z = zz; w = 1;
	}
	P3d(const P3d & o) {
		x = o.x; y = o.y; z = o.z; w = o.w;
	}
  ~P3d() {}

	inline void SetP(T xx, T yy, T zz) {    // w = 1 -> punto
		x = xx; y = yy; z = zz; w = (T) 1;
	}
	inline void SetV(T xx, T yy, T zz) {    // w = 0 -> vettore
		x = xx; y = yy; z = zz; w = (T) 0;
	}
  inline T Modulo( ) { // modulo o dist. origine
  	double m = x*x + y*y + z*z; return (T) sqrt(m);
  }
  inline void Normalizza() {
  	double m = x*x + y*y + z*z; T q = (T) sqrt(m);
    if( q >= (T) 0.00001 ) { x /= q; y /= q; z /= q; }
    w = 0;
  }
	P3d & operator=(const P3d & o) {
		if( this == &o ) return *this;
		x = o.x; y = o.y; z = o.z; w = o.w; return *this;
	}
};


typedef P3d<float> P3f;

///////////////////////////////////////////////////////////////////////

//  Definizione di un vertice
typedef struct UnVertice {
  float Px, Py, Pz;     // coordinate 3d
  float Nx, Ny, Nz;     // normale
  float u, v;           // coord. texture o

  struct UnVertice() {}
  inline void SetP(float x,float y,float z) { Px = x; Py = y; Pz = z; }
  inline void SetN(float x,float y,float z) { Nx = x; Ny = y; Nz = z; }
} Vertex;


//  definizione di un triangolo
typedef struct UnTriangolo {
  int ind[3];           // indici dei 3 vertici CCW (di Vt)
} TPol;

struct UnaSfera {
  std::vector<Vertex> Vt;   // vertici
  std::vector<TPol> Tt;     // poligoni (triangoli)
};

static UnaSfera Sfera;

#define PI  3.141592654

///////////////////////////////////////////////////////////////////////
//  Genera sfera
//  Centro in 0,0,0, raggio dato (r)
//  sec controlla il dettaglio (numero paralleli poli compresi, >= 3)
//  sec    7   triangoli   210
//  sec   17   triangoli  1190
//  sec   31   triangoli  3906
//  sec   91   triangoli 33306
void CreateSphere( float r, int sec )
{
  int i, j, p, k;

  //  Azzero dati eventualmente gia` presenti
  Sfera.Vt.clear();
  Sfera.Tt.clear();

  if( sec < 3 ) return;

  //////////////////////  VERTICI
  //  Un vertice per ciascun polo,
  //  nPar paralleli (poli esclusi)
  //  nV vertici per parallelo (ultimo = primo)
  int nV = sec * 2 + 2;      // vertici per parallelo
  int nPar = sec;            // n. paralleli (poli compresi)
  //  Vertici
  int nVer = nV * nPar + 2;
  Vertex *OrVer = new Vertex [nVer];
  //  polo Nord
  OrVer[0].SetP(0,r,0); OrVer[0].SetN(0,1,0);
  //  Paralleli
  double a = 2.0 * PI / (nV-1);
  double x, y, z, d;
  j = 1;
  P3f NN;
  for(p=0; p < nPar; p++) {
    double t = (PI / (nPar+1)) * (double) (p+1);
    y = r * cos(t); d = r * sin(t);
    for(i=0; i < nV; i++) {
      x = d * cos(a*i); z = d * sin(a*i);
      OrVer[j].SetP(x, y, z);
      NN.SetV(x,y,z); NN.Normalizza();
      OrVer[j].SetN(NN.x,NN.y,NN.z);
      j++;
    }
  }
  //  Polo Sud
  j = nVer-1;
  OrVer[j].SetP(0, -r, 0); OrVer[j].SetN(0,-1,0);

  //////////////////////  TRIANGOLI
  //  Indici
  int nTri = (nV-1)*2;          // poli
  nTri += (nV-1)*(nPar-1);      // strips 1
  nTri += (nV-1)*(nPar-1);      // strips 2
  int *Ind = new int [nTri * 3];
  int *q = Ind;
#define SET_TRI2(a,b,c)  { q[p++]=(a); q[p++]=(b); q[p++]=(c); }
  //  Polo nord
  p=0;
  for(i=0; i < nV-1; i++) SET_TRI2(0,i+2,i+1);

  //  strips n. 1
  int h, l;
  for(h=0; h < nPar-1; h++) {
    for(i=0; i < nV-1; i++) {
      j = i+h*nV+1; k = j+nV; l = k+1; SET_TRI2(j,l,k);
    }
  }
  //  strips n. 2
  for(h=0; h < nPar-1; h++) {
    for(i=0; i < nV-1; i++) {
      j = i+h*nV+1; k = j+nV; l = k+1; k=j+1; SET_TRI2(j,k,l);
    }
  }
  //  Polo sud
  h = nVer-1;
  for(i=0; i < nV-1; i++) {
    k = h-nV+i; j = k+1; SET_TRI2(h,k,j);
  }

  //////////////////////  TEXTURE
  //  Indici della texture
#define SET_TX1(g,tu,tv) { OrVer[g].u = 1.0f-tu; OrVer[g].v = 1.0f-tv; }
  float U, V;
  SET_TX1(0,0.5f,0.0f);      // polo nord
  j = 1;
  for(h=0; h < nPar; h++) {
    V = ((float) (h+1)) / ((float) (nPar+1));
    for(i=0; i < nV; i++) {
      U = ((float) i) / ((float) (nV-1));
        //U *= 2.0f; if( U > 1.0f ) U = 2.0f - U; // simmetrica!
      SET_TX1(j,U,V); j++;
    }
  }
  h = nVer-1;
  SET_TX1(h,0.5f,1.0f);      // polo sud

  //  Copia sulle uscite e clean
  for(i=0; i < nVer; i++) Sfera.Vt.push_back(OrVer[i]);
  delete [] OrVer; OrVer = NULL;
  for(i=0; i < nTri; i++) {
    TPol t;
    t.ind[0] = Ind[i*3];
    t.ind[1] = Ind[i*3+1];
    t.ind[2] = Ind[i*3+2];
    t.ind[3] = 0;
    Sfera.Tt.push_back(t);
  }
  delete [] Ind; Ind = NULL;

}


//  Disegna la sfera memorizzata
void DrawSphere(float r)
{
  if( Sfera.Vt.size() < 2 ) {   // da creare, ne creo una
    CreateSphere(r,17);
  }

  //  Disegno
  glBegin(GL_TRIANGLES);
  for(int i=0; i < Sfera.Tt.size(); i++) {
    for(int k=0; k < 3; k++) {
      int j = Sfera.Tt[i].ind[k];
      glTexCoord2f(Sfera.Vt[j].u, Sfera.Vt[j].v);
     	glNormal3f( Sfera.Vt[j].Nx, Sfera.Vt[j].Ny, Sfera.Vt[j].Nz);
      glVertex3f( Sfera.Vt[j].Px, Sfera.Vt[j].Py, Sfera.Vt[j].Pz);
    }
  }
  glEnd(); 
}