
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Darwin.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

int delete_species(int sid);
/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void)
{
	return 1;
}

/**
 * @brief insert new species in Species' list with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_species (int sid)
{
	struct Species *p;
    struct Species *tmp=Species_head;
    struct Species *temp=Species_head;
    p=malloc(sizeof(struct Species));
    p->Population_head=NULL;
    p->Population_tail=NULL;
    p->next=NULL;
    p->prev=NULL;
    p->sid=sid;
    
    if(Species_head==NULL){ // eisagwgi prwtou stoixeiou
        Species_head=p;
        Species_tail=p;
    }
    else
    {
        if(Species_head->sid>sid){ //eisagwgi stin arxi
            p->next=Species_head;
            Species_head->prev=p;
            Species_head=p;
        }
        else
        {
            while(tmp!=NULL&&tmp->sid<sid){
                temp=tmp;
                tmp=tmp->next;
            }
            if(tmp==NULL){ //eisagwsi sto telos
                temp->next=p;
                p->prev=temp;
                Species_tail=p;
            }
            else
            { //eisagwgi kapou endiamesa
                p->next=temp->next;
                p->prev=tmp->prev;
                temp->next=p;
                tmp->prev=p;
            }
        }
    }
    
    tmp=Species_head;
    int i=1;
    printf("S<%d>\n",sid);
     while (tmp!=NULL)
    {
        printf("  S<%d>\n",tmp->sid);
        if(tmp->Population_head!=NULL){
            struct Population *tp=tmp->Population_head;
            while(tp!=NULL){
                printf("     G<%d>\n",tp->gid);
                tp=tp->next;
            }
        }
        
        tmp=tmp->next;
    }
    printf("DONE\n");

	return 1;
}

/**
 * @brief insert new population with ID <gid> in Species' with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_population(int gid, int sid, int cid){
	    struct Population *p,*tmp,*temp;
    struct Species *species=Species_head;
    p=malloc(sizeof(struct Population));
    p->cid=cid;
    p->gid=gid;
    p->sid=sid;
    p->next_species=NULL;
    p->next=NULL;
    
    
    while(species!=NULL&&species->sid!=sid){
        species=species->next;
    }
    if(species==NULL){
       // printf("sid doesnt exist\n");
        return 0;
    }
    if(species->Population_head==NULL){//eisagwgi prwtou stoixeiou sto population
        species->Population_head=p;
        species->Population_tail=p;
    }
    else
    {
        if(species->Population_head->gid>gid){//eisagwgi stin arxi tis population
            p->next=species->Population_head;
            species->Population_head=p;
        }
        else
        {
            tmp=species->Population_head;
            while(tmp!=NULL&&tmp->gid<gid){
                temp=tmp;
                tmp=tmp->next;
            }
            if(tmp==NULL){   //eisagwgi sto telos tis population
                temp->next=p;
                species->Population_tail=p;
            }
            else                //eisagwgh kapou endiamesa
                                //edw an telika prepei na uparxei elexos oti uparxei idi
            {   
                if(tmp->gid==gid){
                   // printf("already exists\n");
                    return 0;
                }
                p->next=temp->next;   
                temp->next=p;
            }

        }
    }
    species=Species_head;
    printf("G<%d,%d,%d>\n",gid,sid,cid);
    while(species!=NULL){
         printf("  S<%d>\n",species->sid);
        if(species->Population_head!=NULL){
            struct Population *tp=species->Population_head;
            while(tp!=NULL){
                printf("     G<%d,%d>\n",tp->gid,tp->cid);
                tp=tp->next;
            }
        }
        
        species=species->next;
    }

    printf("DONE\n");
	
	
	return 1;
}

/**
 * @brief merge species with IDs <sid1> and <sid2> into new species with ID <sid3>
 *
 * @return 1 on success
 *         0 on failure
 */
int merge_species(int sid1, int sid2, int sid3){
        struct Species *p,*pSid1=Species_head,*pSid2=Species_head;
    struct Population *sid1Population=NULL,*sid2Population=NULL;
    
    if(!insert_species(sid3)){
        return 0;
    }
    p=Species_head;
    while(p->sid!=sid3)
        p=p->next;

    
    
    while(pSid1!=NULL&&pSid1->sid!=sid1){
        pSid1=pSid1->next;
    }
    sid1Population=pSid1->Population_head;
    if(pSid1==NULL){
        return 0; //node with sid == sid1 doesnt exist;
    }
    while(pSid2!=NULL&&pSid2->sid!=sid2){
        pSid2=pSid2->next;
    }
    sid2Population=pSid2->Population_head;
    if(pSid2==NULL){
        return 0; // node with sid == sid2 doesnt exist;
    }
    //ews edw exw pointers se nodes me sid1 k sid2 kai sta populations tous
    //edw ginete i merge
    struct Population *res=NULL,*tmp=NULL,*f=sid1Population,*s=sid2Population,*temp=res;
    if(f==NULL&&s==NULL){
        delete_species(sid1);
        delete_species(sid2);
        p->Population_head=NULL;
        p->Population_tail=NULL;
        return 1;
    }
    while(f!=NULL&&s!=NULL){
        f->sid=sid3;
        s->sid=sid3;
        if(f->gid<s->gid){
            tmp=f;
            if(res==NULL){
                res=tmp;
                f=tmp->next;
                tmp->next=NULL;
                temp=res;    
            }
            else{
                temp->next=tmp;
                f=f->next;
                tmp->next=NULL;
                temp=temp->next; //temp == san tail tis res px.
            }
        }
        else{
            tmp=s;
            if(res==NULL){
                res=tmp;
                s=tmp->next;
                tmp->next=NULL;
                temp=res;
            }else{
                temp->next=tmp;
                s=s->next;
                tmp->next=NULL;
                temp=temp->next;
            }
        }
    }
    if(f==NULL){
        while(s!=NULL){
            s->sid=sid3;
            if(res==NULL){
                res=s;
                temp=s;
                s=s->next;
            }
            else{
            temp->next=s;
            s=s->next;
            temp=temp->next;
            }
        }
    }else if(s==NULL){
        while(f!=NULL){
            f->sid=sid3;
            if(res==NULL){
                res=f;
                temp=f;
                f=f->next;
            }
            else{
                temp->next=f;
                f=f->next;
                temp=temp->next;
            }
        }
    }
    
    p->Population_head=res;
    p->Population_tail=temp;
    
   
    //edw tha kanw delete ta nodes me sid1 k sid2
    if(pSid1->prev==NULL){
        Species_head=pSid1->next;
        pSid1->next=NULL;
    }
   
    if(pSid1->next==NULL){
        Species_tail=pSid1->prev;
        if(pSid1->prev!=NULL)
         pSid1->prev->next=NULL;  
    }
    
    
    if(pSid1->next!=NULL&&pSid1->prev!=NULL){
        pSid1->prev->next=pSid1->next;
        pSid1->next->prev=pSid1->prev;
        pSid1->next=NULL;
        pSid1->prev=NULL;
    }
     

    if(pSid2->prev==NULL){
        Species_head=pSid2->next;
        pSid2->next=NULL;
    }
    if(pSid2->next==NULL){
        Species_tail=pSid2->prev;
        if(pSid2->prev!=NULL)
         pSid2->prev->next=NULL;
    }
    
    if(pSid2->next!=NULL&&pSid2->prev!=NULL){
        pSid2->prev->next=pSid2->next;
        pSid2->next->prev=pSid2->prev;
        pSid2->next=NULL;
        pSid2->prev=NULL;
    }

    //prints
    printf("M<%d><%d><%d>\n",sid1,sid2,sid3);
    p=Species_head;
    struct Population *tail=NULL;
    while(p!=NULL){
        printf("<%d>\n",p->sid);
        tmp=p->Population_head;
        tail=p->Population_tail;
        if(tmp!=tail)
            printf(" ");
        while(tmp!=tail){
            if(tmp->next==tail)
                printf("<%d>,<%d>\n",tmp->gid,tmp->next->gid);

            else
                printf("<%d>,",tmp->gid);
            tmp=tmp->next;    
        }
       
        p=p->next;
    }
	printf("DONE\n");
	return 1;
}

/**
 * @brief Distribute species' population in continents array
 *
 * @return 1 on success
 *         0 on failure
 */
int distribute(){
	struct Species *tmp=Species_head;
    struct Population *ptmp=NULL;
    struct ContinentPopulation *cpypop=NULL;
    int i=0;
    //initialize
    for(i=0;i<5;i++){
        continents[i]=malloc(sizeof(struct ContinentList));
        continents[i]->sentinel=malloc(sizeof(struct ContinentPopulation));
        continents[i]->sentinel->next=NULL;
        continents[i]->sentinel->gid=-1;  
        continents[i]->populationList=continents[i]->sentinel;
    }
    //copy elements to hashtable
    while(tmp!=NULL)
    {
        ptmp=tmp->Population_head;
        while(ptmp!=NULL){ //
            cpypop=malloc(sizeof(struct ContinentPopulation));
            cpypop->gid=ptmp->gid; // copy gid to the new cell of hashtable
            cpypop->next=continents[ptmp->cid]->populationList; //match element to the rigth cell
            continents[ptmp->cid]->populationList=cpypop; //always put a new element on the first index of the list 0(1);
            ptmp=ptmp->next; 
        }
        tmp=tmp->next;
    }

    printf("D\n");
    for(i=0;i<5;i++){
        printf("Continent %d : ",i);
        cpypop=continents[i]->populationList;
        while(cpypop->gid!=-1){
            if(cpypop->next->gid==-1)
                printf("<%d>",cpypop->gid);
            else     
                printf("<%d>,",cpypop->gid);
            cpypop=cpypop->next;
        }
        printf("\n");
    }
    printf("DONE\n");

	return 1;
}

/**
 * @brief delete population with ID <gid> from species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_population(int gid, int sid){
    struct Species *findSid=Species_head;
    struct Population *findGid=NULL,*temp=NULL;
    while(findSid!=NULL&&findSid->sid!=sid){
        findSid=findSid->next;
    }
    if(findSid==NULL){
        printf("sid doesnt exist\n");
        return 0;
    }
    
    findGid=findSid->Population_head;
    temp=findGid;
    while(findGid!=NULL&&findGid->gid!=gid){
        temp=findGid;
        findGid=findGid->next;
    }
    
    if(findGid==NULL){
        printf("gid doesnt exist\n");
        return 0;
    }
    
    int forPrint=findGid->cid;//gia na kserw ti na kanw print sto continent sto telos
    struct ContinentPopulation *del ,*tmp=NULL;
  if(continents[0]!=NULL){//an den exei ginei distribute
    del = continents[findGid->cid]->populationList; // getting the cell in the hashtable
   //delete from array
   
   if(del->gid==gid){ //delete the first element of the list 
       continents[findGid->cid]->populationList=del->next;
       del->next=NULL;
   }
   else{
        while(del->gid!=gid){//delete the element
            tmp=del;
            del=del->next;
        }
        tmp->next=del->next;
        del->next=NULL;
   }
  }
    //delete from population
    if(findSid->Population_head->gid==gid){//if gid is the first element in population
        if(temp->next==NULL){ //if population has only one element
            findSid->Population_head=NULL;
            findSid->Population_tail=NULL;
        }   
        else{
            findSid->Population_head=temp->next;
            temp->next=NULL;
        }   
    }
    
    else if(findGid->next==NULL){//if the last element in population list has the gid
        findSid->Population_tail=temp;
        temp->next=NULL;
    }
    else {
        temp->next=findGid->next;
        findGid->next=NULL;
    }

    //print

    printf("K<gid><sid>\nSPECIES\n   ");
    printf("<%d>\n   ",findSid->sid);
    temp=findSid->Population_head;
    while(temp!=NULL){
        if(temp->next==NULL)
            printf("<%d>\n",temp->gid);
        else        
            printf("<%d>,",temp->gid);
        temp=temp->next;    
    }
    
    if(continents[0]==NULL){ //an den exei ginei distribute
        printf("DONE\n");
        return 1;
    }

    printf("CONTINENTS\n   Continent<%d>:",forPrint);
    del=continents[forPrint]->populationList;
    while(del->gid!=-1){
        if(del->next->gid==-1)
            printf("<%d>\n",del->gid);
        else    
            printf("<%d>,",del->gid);
        del=del->next;
    } 
    printf("DONE\n");


	return 1;
}

/**
 * @brief delete species with ID <sid> and its populations
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_species(int sid){
	struct Species *tail=Species_tail;
    while(tail!=NULL&&tail->sid!=sid){
        tail=tail->prev;
    }
    if(tail==NULL){
        printf("sid doesnt exist\n");
        return 0;
    }
    struct Population *pHead=tail->Population_head;
    while(pHead!=NULL){
        delete_population(pHead->gid,tail->sid); //delete panta to prwto stoixeio tis population
        pHead=tail->Population_head;    //panta sto kenourio population head afou to proigoumeno ginete null meta tin delete
    }
    //delete species
    
    if(tail->next==NULL){
        Species_tail=tail->prev;
        tail->prev=NULL;
        if(Species_tail!=NULL)//an den einai to teleutaio stoixio sti lista
            Species_tail->next=NULL;
        else
            Species_head=NULL;//periptosi diagrafis teleutaiou stixiou
        
    }
    else if(tail->prev==NULL){
        Species_head=tail->next;
       if(Species_head!=NULL)
            Species_head->prev=NULL;
        
            
    }
    else{
        tail->prev->next=tail->next;
        tail->next->prev=tail->prev;
        tail->next=NULL;
        tail->prev=NULL;
    }
    
    //prints
    printf("\nF<%d>\nSPECIES\n",sid);
    struct Species *head=Species_head;
    while(head!=NULL){
        printf("<%d>\n",head->sid);
        pHead=head->Population_head;
        if(pHead!=NULL)
            printf(" ");
        while(pHead!=NULL){
            if(pHead->next==NULL)
                printf("<%d>\n",pHead->gid);
            else
                printf("<%d>,",pHead->gid);
            pHead=pHead->next;
        }
        head=head->next;
    }
    printf("CONTINENTS\n");
    int i=0;
    for(i=0;i<5;i++){
        printf("   Continent %d : ",i);
       if(continents[0]!=NULL){
        struct ContinentPopulation *tmp=continents[i]->populationList;
        while(tmp->gid!=-1){
            if(tmp->next->gid==-1)
                printf("<%d>",tmp->gid);
             else
                printf("<%d>,",tmp->gid);
            tmp=tmp->next;   

        }
       }
       printf("\n");
    }

    printf("\nDONE\n");
	
	return 1;
}

/**
 * @brief Remaining species evolve into homo sapiens.
 *
 * @return 1 on success
 *         0 on failure
 */
int evolution(){
	struct Species *head=Species_head;
    struct Population *pHead=NULL,*homoNext=NULL,*speciesNext=NULL;
    hs_L=NULL;
    if(head==NULL){
        return 0;
    }

    while(head!=NULL){
        if(head->Population_head!=NULL){
            if(hs_L==NULL){ // initialize mia fora mono  
                hs_L=head->Population_head;
            }
            else{
                speciesNext->next_species=head->Population_head;//to first node tis listas na deixnei sto epomeno eidos
                homoNext->next=head->Population_head; //teleutaio node tis population enonete me to prwto tis epomenis population
            }
            homoNext=head->Population_tail; //na deixnei stin oura tou proigoumenou population  ^^^^^
            speciesNext=head->Population_head; //na deixnei sto head tou proigoumenou population
        }
        head=head->next;
    }
    if(hs_L==NULL){
        return 0;
    }
    
    pHead=hs_L;
    homoNext=pHead->next_species;
    printf("E\n  Homo Sapiens: [");
    while(pHead!=NULL){
        if(pHead->next!=homoNext){
            printf("<%d,%d>,",pHead->gid,pHead->sid);
        }else{
            if(pHead->next!=NULL){
                printf("<%d,%d>][",pHead->gid,pHead->sid);
                homoNext=pHead->next->next_species;
            }   
            else
                printf("<%d,%d>]",pHead->gid,pHead->sid);
        }
        pHead=pHead->next;
    }
    printf("\nDONE\n");



	return 1;
}

/**
 * @brief Gather statistics on species from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int species_statistics(){
	 struct Population *homo=hs_L;
    int sum=0;
    if(homo==NULL){
        return 0;
    }
    while(homo!=NULL){
        sum++;
        homo=homo->next_species;
    }
    printf("N\n Homo Sapiens species:<%d>\nDONE\n",sum);

	return 1;
}

/**
 * @brief Gather statistics on population belonging to species with ID <sid> from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int population_statistics(int sid){
	struct Population *homo=hs_L,*tmp=NULL;
    int sum=0;
    
    
    while(homo!=NULL&&homo->sid!=sid)
        homo=homo->next_species;

    if(homo==NULL){
        printf("J<%d>\n Homo Sapiens population:<%d>\nDONE\n",sid,sum);
        return 0;
    }
    tmp=homo->next_species;//tmp einai to bound
    while(tmp!=homo){  //otan xtipisei to epomeno eidos stamataei
        sum++;
        homo=homo->next;
    }

    printf("J<%d>\n Homo Sapiens population:<%d>\nDONE\n",sid,sum);

	return 1;
}

/**
 * @brief Print species' list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_species(){
    struct Species *head=Species_head;
    printf("P\n ");
    while(head!=NULL){
        if(head->next==NULL)
            printf("<%d>\n",head->sid);
        else
            printf("<%d>,",head->sid);

        head=head->next;
    }
	printf("DONE\n");
	return 1;
}

/**
 * @brief Print species' list with population lists
 *
 * @return 1 on success
 *         0 on failure
 */
int print_populations(){
	struct Species *head=Species_head;
    struct Population* tmp=NULL,*tail=NULL;
    printf("X\n");
    while(head!=NULL){
        printf("<%d>\n",head->sid);
        tmp=head->Population_head;
        tail=head->Population_tail;
        while(tmp!=tail){ //giati exoun xalasei oi upolistes meta tin evo()
            printf(" ");
            if(tmp->next==tail)
                printf("<%d,%d>, <%d,%d>\n",tmp->gid,tmp->cid,tmp->next->gid,tmp->next->cid);
            else
                printf("<%d,%d>,",tmp->gid,tmp->cid);
            tmp=tmp->next;    
        }  
        head=head->next;
    }
    printf("DONE\n");
	return 1;
}

/**
 * @brief Print continents array with each continent's population list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_continents(){
    struct ContinentPopulation *cpypop=NULL;
    int i=0;
    printf("C\n");
    for(i=0;i<5;i++){
        printf("Continent %d : ",i);
       if(continents[0]!=NULL){
        cpypop=continents[i]->populationList;
        while(cpypop->gid!=-1){
            if(cpypop->next->gid==-1)
                printf("<%d>",cpypop->gid);
            else     
                printf("<%d>,",cpypop->gid);
            cpypop=cpypop->next;
        }
       }
        printf("\n");
    }
    printf("DONE\n");
	return 1;
}

/**
 * @brief Print homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_homo_sapiens(){
    struct Population *pHead=NULL,*homoNext=NULL;
    pHead=hs_L;
    if(pHead!=NULL)
        homoNext=pHead->next_species;
    printf("E\n  Homo Sapiens: [");
    while(pHead!=NULL){
        if(pHead->next!=homoNext){
            printf("<%d,%d>,",pHead->gid,pHead->sid);
        }else{
            if(pHead->next!=NULL){
                printf("<%d,%d>][",pHead->gid,pHead->sid);
                homoNext=pHead->next->next_species;
            }   
            else
                printf("<%d,%d>]",pHead->gid,pHead->sid);
        }
        pHead=pHead->next;
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void)
{
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Insert Species
				 * S <sid> */
			case 'S':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (insert_species(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Insert population
				 * G <gid> <sid> <cid> */
			case 'G':
				{
					int gid, sid, cid;

					sscanf(buff, "%c %d %d %d", &event, &gid, &sid, &cid);
					DPRINT("%c %d %d %d\n", event, gid, sid, cid);

					if (insert_population(gid, sid, cid)) {
						DPRINT("%c %d %d %d succeeded\n", event, gid, sid, cid);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, gid, sid, cid);
					}

					break;
				}

				/* Merge species
				 * M <sid1> <sid2> <sid3> */
			case 'M':
				{
					int sid1, sid2, sid3;

					sscanf(buff, "%c %d %d %d", &event, &sid1, &sid2, &sid3);
					DPRINT("%c %d %d %d\n", event, sid1, sid2, sid3);

					if (merge_species(sid1, sid2, sid3)) {
						DPRINT("%c %d %d %d succeeded\n", event, sid1, sid2, sid3);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, sid1, sid2, sid3);
					}

					break;
				}

				/* Distribute species
				 * D */
			case 'D':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (distribute()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Delete population
				 * K <gid> <sid> */
			case 'K':
				{
					int gid, sid;

					sscanf(buff, "%c %d %d", &event, &gid, &sid);
					DPRINT("%c %d %d\n", event, gid, sid);

					if (delete_population(gid, sid)) {
						DPRINT("%c %d %d succeeded\n", event, gid, sid);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, gid, sid);
					}

					break;
				}

				/* Delete species
				 * F <sid> */
			case 'F':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (delete_species(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Evolution to homo sapiens
				 * E */
			case 'E':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (evolution()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Species' statistics
				 * N */
			case 'N':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (species_statistics()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Population statistics
				 * J <sid> */
			case 'J':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (population_statistics(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Print species
				 * P */
			case 'P':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_species()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print populations
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_populations()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print continents
				 * C */
			case 'C':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_continents()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print homo sapiens
				 * W */
			case 'W':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_homo_sapiens()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
