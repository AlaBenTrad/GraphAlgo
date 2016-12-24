

#include <graphics.h>
#include <conio.h>
#include<stdio.h>
#include<stdlib.h>
#define Int_Max 50

    int visited[Int_Max],q[Int_Max];
    int N,f=0,r=-1,Col=0,nbre_noeud_visite;
    int G[Int_Max][Int_Max];
    FILE *Fi=NULL;
    FILE *Fo=NULL;
    int Tab[50],Tab1[50],plus_court[50];
    int k=0,p=0;
    typedef struct
    {
       int index;
       int degree;
       int  color;
    }node;
    node vertices[50];

void permutation (node* n1,node* n2){
    node temp;
    temp=*n1;
    *n1=*n2;
    *n2=temp;
}

void Welsh_Powell(int G[Int_Max][Int_Max],int n) {
     int i,j,k,non_colore,found,firstUncoloured,allColored;
     int  c;
    for(i=0; i<n; i++){
         //initialisiation de case tableau
         vertices[i].index=i;
         vertices[i].color=0;
         vertices[i].degree=0;
         for(j=0; j<n; j++)
             if(G[i][j]!=0)
                vertices[i].degree++;
     }
     //tri selection
     for(i=0; i<n-1; i++)
        for(j=i+1; j<n; j++)
            if(vertices[i].degree<vertices[j].degree)
                permutation(&vertices[i],&vertices[j]);

    for(c=1; c<=N; c++){ //for 1
        //chercher les noeuds incoloré
        firstUncoloured=0;
        i=0;
        while((i<n)&&(firstUncoloured==0)){
            if(vertices[i].color==0)
                firstUncoloured=1;
            else
                i++;
        }
        vertices[i].color=c;
        //chercher les noeud non adjacents pr attribuer la même couleur
        for(i=0;i<n;i++){ //for 2
            if(vertices[i].color==0){
                //si le noeud est non coloré
                non_colore=1;
                for(j=0;(j<n)&&(non_colore==1); j++){
                    found=0;
                    //on cherche l'indice de noeud
                    for(k=0; (k<n)&&(found==0); k++)
                        if(vertices[k].index==j)
                            found=1;
                    if(found==1)
                        if((vertices[k-1].color==c)&&((G[vertices[i].index][j]!=0)||(G[j][vertices[i].index]!=0)))
                            non_colore=0;
                }
                if(non_colore==1)
                    vertices[i].color=c;
            }
        } //end for 2
    }//end for 1

    //tri chromatique tous les noeuds qui ont la même couleur seront adjacents

    for(i=0;i<N-1;i++)
        for(j=i+1;j<N; j++)
            if(vertices[i].color<vertices[j].color)
                 permutation(&vertices[i],&vertices[j]);

    //calculer le nombre des couleurs
    Col=0;
    for(i=0;i<n;i++)
        if(vertices[i].color != vertices[i+1].color)
            Col++;

    //affichage
    fprintf(Fo,"\nLe nombre chromatique du graphe est %d.",Col);
}

int printSolution(int dist[], int src, int dst,int ch[],int x){
    int p;
    //dans le cas ou il n'existe pas de court chemin
    if((dist[dst]==Int_Max)||(dist[src]==Int_Max))
        fprintf(Fo,"\nNO\n");
    else {
        fprintf(Fo,"\nLe plus court chemin (1");
        plus_court[0]=src+1;
        for(p=x;p>=0;p--)
            fprintf(Fo,"-%d",ch[p]+1);
        int i;
        for(i=x;i>=0;i--)
            plus_court[x-i+1]=ch[i]+1;
        plus_court[x+1]=dst+1;
        nbre_noeud_visite=x+1;
        fprintf(Fo,") entre les sommets 1 et %d du graphe est ",N);
        fprintf(Fo,"%i.\n",dist[dst]-dist[src]);
    }

}

int minDistance(int dist[], int visited[]){
   int min = Int_Max, min_index,i;
   for (i=0; i<N; i++){
        // S'il y a une distance inférieure à min et que le noeud correspondant n'a pas été visité, on considère ce noeud ainsi que son indice
       if ((visited[i]==0) && (dist[i] < min)){
          min = dist[i];
          min_index=i;
       }
   }
   return min_index;
}

void dijkstra(int src, int dst){
     int dist[N]; //tableau des distances
     int visited[N]; //tab booléen (visiter ==1 ) (non visiter ==0)
     int i,j,u;
     for (i = 0; i < N; i++){
        dist[i] = Int_Max; // Initialisation des cases de dist à une grande valeur (INT_MAX)
        visited[i] = 0; //tous les noeuds ne sont pas visités
     }

     dist[src] = 0; // La distance d'un noeud avec lui-même est nulle

     //remplir tableau des distances
     for (i = 0; i < N-1; i++) {
       u=minDistance(dist, visited);
       visited[u]=1;
       for (j = 0; j < N; j++)
            if ( ( visited[j]==0 ) && ( G[u][j] !=0 ) && ( dist[u] != Int_Max ) && ( dist[u]+G[u][j] < dist[j]) )
            {
                //etablir la mise a jour de tableau distance en cas ou la distance est plus courte que la derniere
               dist[j] = dist[u] + G[u][j];
               Tab[j]=u;
            }
    }
    /*
    for(i=0;i<N+1;i++)
        printf("%i ",Tab[i]);
    printf("\nx v i ch[i]\n");
    */
    int v,ch[N],x;
    v=N-1;
    ch[0]=N-1;
    i=0;
    while (Tab[v]>0){
        x=v;
        v=Tab[x];
        i++;
        ch[i]=v;
        //printf("%i %i %i %i     %i\n",x,v,i,ch[i],Tab[v]);
    }
    printSolution(dist, src, dst,ch,i);
}

void dfs(int v){
    int i;
    fprintf(Fo,"%d ",v+1);
    //marquer le noeuds comme visité
    visited[v]=1;
    for(i=0;i<N;i++) //calculer les noeds non visié et adjacents a v
        if((G[v][i]!=0)&&(visited[i]==0))
            dfs(i);
}

void DFS(int v){
    int i;
    //initialisation : tous les noeuds ne sont pas visités
    for(i=0;i<N;i++)
        visited[i]=0;
    fprintf(Fo,"\nDFS(%i)\n",v);
    dfs(v-1);
    fprintf(Fo,"\n");
}

void bfs(int v){
    int i;
    for(i=0;i<N;i++){
        if((G[v][i]!=0)&&(visited[i]==0))
            q[++r]=i;
    }
    if(f<=r){
        visited[q[f]]=1;
        bfs(q[f++]);
    }
}

void BFS(int v){
    int i;
    for(i=0;i<N;i++){
        q[i]=0;
        visited[i]=0;
    }
    fprintf(Fo,"\nBFS(%i)\n%i ",v,v);
    f=0;
    r=-1;
    bfs(v-1);
    int reached[50]={0};
    reached[v-1]=1;
    for(i=0;i<f;i++){
        if(reached[q[i]]==0){
            fprintf(Fo,"%d ",q[i]+1);
            reached[q[i]]=1;
        }
    }
    fprintf(Fo,"\n");
}

//initioalisation matrice d'adjacance
void initgraph(int G[Int_Max][Int_Max]){
    int i,j;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            G[i][j]=0;
}

void traitement (){
    int D,E,A,B,C,i,j;
    initgraph(G);
    Fi = fopen("input.txt","r");
    if(Fi != NULL)
    {
          fscanf(Fi,"%d%d%d",&D,&N,&E);
          for(i=0;i<E;i++){
                fscanf(Fi,"%d%d%d",&A,&B,&C);
                G[A-1][B-1]=C;
                if(D==0)
                    G[B-1][A-1]=C;
          }
        fclose(Fi);
    }
    Fo=fopen("output.txt","w");
    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
            fprintf(Fo,"%d ",G[i][j]);
        fprintf(Fo,"\n");
    }
    DFS(1);
    DFS(N);
    BFS(1);
    BFS(N);
    dijkstra(0,N-1);
    Welsh_Powell(G,N);
    fclose(Fo);
    //Affichage de cont�nue de fichier texte
    Fo=fopen("output.txt","r");
    char ch[100];
    while(!feof(Fo)){
        fgets(ch,100,Fo);
        printf("%s",ch);
    }
    fclose(Fo);
}

void graphique (){
    int gd = DETECT, gm;
   initgraph(&gd, &gm, "C:\\TC\\BGI");
   char ch[10];
    int ty[]={2,8,2,4,7,3,8,4,6,9},tx[]={9,7,4,9,2,7,5,1,10,1},j,i,font = 10,couleur;
   //circle(x,y,radiuis)
   settextstyle(font,HORIZ_DIR, 1);
   setcolor(YELLOW);
   outtextxy(170,20,"****  PROJET GRAPHE  ****");
   setcolor(WHITE);
    char ch1[3];
    settextstyle(3,HORIZ_DIR,1);
    setcolor(WHITE);
    for(i=0;i<N;i++){
        sprintf(ch,"Node %i",i+1);
        //sprintf(ch,"%c",65+i);
        outtextxy(tx[i]*50-40,ty[i]*50+15,ch);
        //circle(tx[i]*50,ty[i]*50,20);
        arc(tx[i]*50,ty[i]*50,360,0,15);
        delay(120);
    }
    couleur=1;
   for(i=0;i<N;i++){
        //couleur++;
        for(j=0;j<N;j++)
            if(G[i][j]!=0){
                setcolor(couleur%15);
                line(tx[i]*50,ty[i]*50,tx[j]*50,ty[j]*50);
                couleur++;
                while(couleur==0||couleur==15)
                    couleur++;
                delay(120);
                //setcolor(WHITE);
                itoa(G[i][j],ch,10);
                outtextxy((tx[i]+tx[j])*25-20,(ty[i]+ty[j])*25-20,ch);
                //car le milieu de deux points = (x1+x2)/2  cordnnée (y1+y2)/2
            }
   }
   setcolor(YELLOW);
   setlinestyle(0,0,3);
    for(i=1;i<=nbre_noeud_visite;i++){
        line(tx[plus_court[i]-1]*50,ty[plus_court[i]-1]*50,tx[plus_court[i-1]-1]*50,ty[plus_court[i-1]-1]*50);
        itoa(G[plus_court[i]-1][plus_court[i-1]-1],ch,10);
        outtextxy((tx[plus_court[i]-1]+tx[plus_court[i-1]-1])*25-20,(ty[plus_court[i]-1]+ty[plus_court[i-1]-1])*25-20,ch);
        delay(120);
   }
   for(i=0;i<N;i++) // On parcourt les colonnes de la matrice G
         {
            setcolor(vertices[i].color+2);
            sector(tx[i]*50,ty[i]*50,0,360,15,15);
            //disk : 360°
            delay(120);
        }
   setcolor(YELLOW);
   settextstyle(4,HORIZ_DIR,1);
   //settextstyle(font,HORIZ_DIR,1);
   for(i=1;i<=nbre_noeud_visite+1;i++){
        itoa(i,ch,10);
        setbkcolor(vertices[plus_court[i-1]-1].color+2);
        //pour avoir le meme couleur de cercle et le fond de numéro
        outtextxy(tx[plus_court[i-1]-1]*50-10,ty[plus_court[i-1]-1]*50-10,ch);
        delay(120);
   }
   getch();
   closegraph();
}

int main (){
    traitement();
    graphique();
    system("pause>null");
    return 0;
}

