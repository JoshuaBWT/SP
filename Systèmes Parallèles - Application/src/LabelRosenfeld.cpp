#include "LabelRosenfeld.hpp"

/* Constructeur par defaut */
LabelRosenfeld::LabelRosenfeld() {
}

LabelRosenfeld::~LabelRosenfeld() {
}

/* Fonctions communes */
uint32_t LabelRosenfeld::FindRoot(uint32_t* T, uint32_t i) {
    uint32_t r;
    r = i;
    while (T[r]<r) {
        r = T[r];
    }
    return r;
}
void LabelRosenfeld::SetRoot(uint32_t* T, uint32_t i, uint32_t r) {
    uint32_t j;

    while (T[i]<i) {
        j = T[i];
        i = j;
    }
    T[i] = r;
}
void LabelRosenfeld::updateTable(uint32_t* T, uint32_t e, uint32_t epsillon) {
    uint32_t m;

    if ((!e) || (e == epsillon)) return;

    while (T[e] != epsillon) {
        m = T[e];
        T[e] = epsillon;
        e = m;
    }
}

uint32_t LabelRosenfeld::ui32MinNonNul2(uint32_t a, uint32_t b) {
    uint32_t m;

    if (a) {
        if (b) {
            if (a<b) m = a;
            else m =b;
        } else {
            m = a;
        }
    } else {
        m = b;
    }
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul3(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t m32 = 0xffffffff;
    uint32_t m = m32;

    if (a) m = a;
    if (b && (b<m)) m = b;
    if (c && (c<m)) m = c;

    if (m==m32) m = 0;
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul4(uint32_t a0, uint32_t b0, uint32_t a1, uint32_t b1) {
    uint32_t m, m0,m1;

    m0 = ui32MinNonNul2(a0, b0);
    m1 = ui32MinNonNul2(a1, b1);
    m  = ui32MinNonNul2(m0, m1);

    return m;
}

/* Autres fonctions utiles */
uint32_t LabelRosenfeld::solvePackTable(uint32_t* T, uint32_t ne) {
    uint32_t e;
    uint32_t na; // ancetre pack

    na = 0;
    for (e=1; e <= ne; e++) {
        if (e != T[e]) {
            T[e] = T[T[e]];
        } else {
            T[e] = ++na;
        }
    }
    return na;
}

uint32_t LabelRosenfeld::solvePackTable(Region32* region32, uint32_t nes[]) {
    uint32_t e;
    uint32_t na; // ancetre pack

    uint32_t * T = region32->T;

    for(int i = 0; i < region32->Regions.size(); i++)
    {
        na = solvePackTable(region32->Regions[i].T, nes[i]);
    }
    /*
    na = 0;
    for (e=1; e<=ne; e++) {
        if (e != T[e]) {
            T[e] = T[T[e]];
        } else {
            T[e] = ++na;
        }
    }
    */
    return na;
}

void LabelRosenfeld::updateLabel(uint32_t **E, int i0, int i1, int j0, int j1, uint32_t* T) {
    int i, j;

    for (i=i0; i<i1; i++) {
        for (j=j0; j<j1; j++) {
            E[i][j] = T[E[i][j]];
        }
    }
}

uint32_t LabelRosenfeld::line0Labeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4, r4;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i  ][j-1];

            if (e4 == 0) {
                E[i][j] = ++ne;
            } else {
                r4 = FindRoot(T, e4);
                E[i][j] = r4;
            }
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e2, e4;
    uint32_t r2, r4;

    // premier point de la ligne
    x = X[i][0];

    if (x) {
        e2 = E[i-1][0];
        if (e2) {
            E[i][0] = e2;
        } else {
            E[i][0] = ++ne;
        }
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e2 = E[i-1][j  ];
            e4 = E[i  ][j-1];

            if ( (e2 == 0) && (e4 == 0) ) {
                E[i][j] = ++ne;
            } else {
                if (e2 == e4) {
                    E[i][j] = e2;
                } else {
                    r2 = FindRoot(T, e2);
                    r4 = FindRoot(T, e4);
                    epsillon = ui32MinNonNul2(r2, r4);

                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);
                    E[i][j] = epsillon;
                }
            }
        }
    }
    return ne;
}

uint32_t LabelRosenfeld::line0Labeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4,e;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i][j-1];
            if (e4) {
                e = FindRoot(T,e4);
            } else {
                e = ++ne;
            }
            E[i][j] = e;
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e1, e2, e3, e4;
    uint32_t r1, r2, r3, r4;

    // premier point de la ligne
    x = X[i][0];
    if (x) {
        e2 = E[i-1][0];
        e3 = E[i-1][1];

        if ( (e2==0) && (e3==0)) {
            E[i][0] = ++ne;
        } else {
            if (e2==e3) {
                E[i][0] = e2;
            } else {
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);

                epsillon = ui32MinNonNul2(r2, r3);

                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);

                E[i][0] = epsillon;
            }
        }
    }

    for (j=1; j<largeur-1; j++) {
        x = X[i][j];
        if (x)  {
            e1 = E[i-1][j-1];
            e2 = E[i-1][j  ];
            e3 = E[i-1][j+1];
            e4 = E[i  ][j-1];

            // nouvel element
            if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
                E[i][j] = ++ne;
            } else {
                if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                    E[i][j] = e1;
                } else {

                    r1 = FindRoot(T,e1);
                    r2 = FindRoot(T,e2);
                    r3 = FindRoot(T,e3);
                    r4 = FindRoot(T,e4);

                    epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                    if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                    E[i][j] = epsillon;
                }
            }
        }
    }

    j = largeur-1;
    x = X[i][j];
    if (x)  {
        e1 = E[i-1][j-1];
        e2 = E[i-1][j  ];
        e3 = 0;
        e4 = E[i  ][j-1];

        // nouvel element
        if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
            E[i][j] = ++ne;

        } else {
            if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                E[i][j] = e1;
            } else {

                r1 = FindRoot(T,e1);
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);
                r4 = FindRoot(T,e4);

                epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                E[i][j] = epsillon;
            }
        }
    }
    return ne;
}



/* Labelise en sequentiel */
void LabelRosenfeld::labeliseSequetiel4C(Region32& region32) {

    /* Declaration des variables */
    int i;
    uint32_t ne;

    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des précédents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;
    ne = line0Labeling4C(region32.X, i0, region32.E, region32.T, largeur, ne);

    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling4C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* Résolution des équivalences */
    //region32.neFinal = solvePackTable(region32.T, ne);

    /* Mise à jour sur l'image */
    updateLabel(region32.E, i0, i1, j0, j1, region32.T);

    /* Mémorisation du nombre d'étiquettes */
    region32.ne = ne;
}
void LabelRosenfeld::labeliseSequetiel8C(Region32& region32) {

    /* Declaration des variables */
    int i;
    uint32_t ne;

    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des précédents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;

    ne = line0Labeling8C(region32.X, i0, region32.E, region32.T, largeur, ne);
    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling8C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* Résolution des équivalences */
    region32.neFinal = solvePackTable(region32.T, ne);

    /* Mise à jour sur l'image */
    updateLabel(region32.E, i0, i1, j0, j1, region32.T);

    /* Mémorisation du nombre d'étiquettes */
    region32.ne = ne;
}




/* Labelise en parallèle */
void LabelRosenfeld::labeliseParallele4C(Region32& region32) {
    /* Declaration des variables */
    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    uint32_t ne;

    /* Netoyage des précédents traitements */
    region32.cleanRegions32();
    /* Premier etiquetage */

    ne = 0;
    //cout << "number of regions :" << region32.Regions.size() << endl;
    //ne = line0Labeling4C(region32.Regions[0].X, i0, region32.Regions[0].E, region32.Regions[0].T, largeur, ne);
    for(int j = 0; j < region32.Regions.size(); j++) {
        Region32 region32Bis = region32.Regions[j];
        i0 			= 	region32Bis.i0;
        i1 			= 	region32Bis.i1;
        ne = j;
        ne = line0Labeling4C(region32Bis.X, i0, region32Bis.E, region32Bis.T, largeur, ne);
        for (int i=i0+1; i<i1; i++) {
            ne = lineLabeling4C(region32Bis.X, i, region32Bis.E, region32Bis.T, largeur, ne);
        }
        updateLabel(region32Bis.E, i0, i1, j0, j1, region32Bis.T);
    }

    region32.neFinal = solvePackTable(region32.T, ne);

    updateLabel(region32.E, i0, i1, j0, j1, region32.T);
    //updateLabel(region32Bis.E, i0, i1, j0, j1, region32Bis.T);
    /* Résolution des équivalences */
    /* Mise à jour sur l'image */
    region32.ne = ne;
    /* Mémorisation du nombre d'étiquettes */
}

void LabelRosenfeld::labeliseParallele8C(Region32& region32) {
    //LabelRosenfeld::labeliseParallele4C(region32);
}
