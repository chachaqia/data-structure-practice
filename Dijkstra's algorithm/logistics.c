// Starting code for Dijkstra's algorithm ... COMP9024 22T3

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

typedef struct city_node {
    int node_num;
    char city_name[33];

} city_node;

city_node dictionary[9999];
int city_to_city_min[9999][9999];

int hub_plan[9999];
int pointer=0;
int hub_num;
int continue_yes_or_no = 1;


void showPath(int v, int pred[], int nV) {
    int j;
    city_node tem_city;
    if (pred[v] == -1) {
        for(j = 0; j < nV; j++){
            if (dictionary[j].node_num == v){
                tem_city = dictionary[j];
                break;
            }
        }
        printf("%s ", tem_city.city_name);
    } else {
        showPath(pred[v], pred, nV);
        for(j = 0; j < nV; j++){
            if (dictionary[j].node_num == v){
                tem_city = dictionary[j];
                break;
            }
        }
        printf("- %s", tem_city.city_name);
    }
}

void dijkstraSSSP(Graph g, int best_hub[][3]) {
    int  dist[MAX_NODES];
    int  pred[MAX_NODES];
    bool vSet[MAX_NODES];  // vSet[v] = true <=> v has not been processed
    int s, t, i;
    int nV = numOfVertices(g);

    for (i = 0; i < nV; i++) {
        PQueueInit();
        for (s = 0; s < nV; s++) {
            joinPQueue(s);
            dist[s] = VERY_HIGH_VALUE;
            pred[s] = -1;
            vSet[s] = true;
        }

        dist[best_hub[i][1]] = 0;
        while (!PQueueIsEmpty()) {
            s = leavePQueue(dist);
            vSet[s] = false;
            for (t = 0; t < nV; t++) {
                if (vSet[t]) {
                    int weight = adjacent(g,s,t);
                    if (weight > 0 && dist[s]+weight < dist[t]) {  // relax along (s,t,weight)
                        dist[t] = dist[s] + weight;
                        pred[t] = s;
                    }
                }
            }
        }
        printf("%s: ", dictionary[i].city_name);
        if(best_hub[i][2] == 0){
            printf("%s 0\n", dictionary[i].city_name);
        } else {
            showPath(best_hub[i][0], pred, nV);
            printf(" %d", best_hub[i][2]);
            putchar('\n');
        }
    }
}

void getInformation(Graph g, Vertex source, int i){
    int  dist[MAX_NODES];
    bool vSet[MAX_NODES];  // vSet[v] = true <=> v has not been processed
    int s, t;

    PQueueInit();
    int nV = numOfVertices(g);
    for (s = 0; s < nV; s++) {
        joinPQueue(s);
        dist[s] = VERY_HIGH_VALUE;
        vSet[s] = true;
    }
    dist[source] = 0;
    while (!PQueueIsEmpty()) {
        s = leavePQueue(dist);
        vSet[s] = false;
        for (t = 0; t < nV; t++) {
            if (vSet[t]) {
                int weight = adjacent(g,s,t);
                if (weight > 0 && dist[s]+weight < dist[t]) {  // relax along (s,t,weight)
                    dist[t] = dist[s] + weight;
                }
            }
        }
    }
    for (s = 0; s < nV; s++) {
        if (dist[dictionary[s].node_num] < VERY_HIGH_VALUE) {
            city_to_city_min[i][s] = dist[dictionary[s].node_num];
        } else {
            city_to_city_min[i][s] = VERY_HIGH_VALUE;
        }
    }
}

// Use recursive implementations of permutations and combinations to generate hub plans
// The following function C code I copied and modified from:
// https://blog.csdn.net/zyx_ly/article/details/106558014?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166744665416782414925215%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166744665416782414925215&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-11-106558014-null-null.142^v62^pc_search_tree,201^v3^add_ask,213^v1^t3_esquery_v3&utm_term=c%E8%AF%AD%E8%A8%80%E4%BB%8Em%E4%B8%AA%E6%95%B0%E4%B8%AD%E4%BB%BB%E9%80%89n%E4%B8%AA%E6%95%B0&spm=1018.2226.3001.4187
void C(int n, int m, int a[], int b[])
{
    int i;
    int j;
    for(i = m; i <= n; i++)
    {
        b[m - 1] = i - 1;
        if(m > 1)
        {
            C(i - 1, m - 1, a, b);
        }
        else
        {
            for(j = 0; j < hub_num; j++)
            {
//                printf("%d ", a[b[j]]);
                hub_plan[pointer]=a[b[j]];
                pointer++;

            }
//            printf("\n");
        }
    }
}

void findHubs(int max_distance, int nV, int nH, Graph graph){
    int i, j, z, arrived_num;
    int print_hub[nH];

    hub_num = nH;
    int a[nV];
    for(i = 0; i < nV; i++)
    {
        a[i] = i;
    }
    int b[nH];
    C(nV, nH, a, b);

    i = 0;
    while(i < pointer){
        //creat an array and initialize it to record the plan can arrive all cities or not
        bool arrived[nV];
        int best_hub[nV][3];
        for(z = 0; z < nV; z++){
            arrived[z] = false;
            best_hub[z][0] = dictionary[z].node_num;
            best_hub[z][1] = -1;
            best_hub[z][2] = VERY_HIGH_VALUE;
        }

        for(j = 1; j <= nH; j++){
            print_hub[j-1] = hub_plan[i];
            for(z = 0; z < nV; z++){
                if(city_to_city_min[hub_plan[i]][z] <= max_distance){
                    arrived[z] = true;
                    if(city_to_city_min[hub_plan[i]][z] < best_hub[z][2]){
                        best_hub[z][2] = city_to_city_min[hub_plan[i]][z];
                        best_hub[z][1] = dictionary[hub_plan[i]].node_num;
                    }
                }
            }
            i++;
        }
        arrived_num = 0;
        for(z = 0; z < nV; z++){
            if(arrived[z] == true) {
                arrived_num++;
            }
        }

        if(arrived_num == nV){
            continue_yes_or_no = 0;
//            for(z=0; z<nV; z++){
//                printf("%d   %d   %d\n",best_hub[z][0],best_hub[z][1],best_hub[z][2]);
//            }

            printf("\nHubs: ");
            for(z = 0; z < nH - 1; z++){
                printf("%s, ", dictionary[print_hub[z]].city_name);
            }
            printf("%s\n", dictionary[print_hub[z]].city_name);
            printf("Routes:\n");


            dijkstraSSSP(graph,best_hub);


        }
    }

}


int main(void){
    int num;
    char start_city[33];
    char end_city[33];
    int weight;

    city_node temp_city_node;
    int i = 0, j;
    int road_num;
    Edge edge;
    int max_distance;

// Receive city information to build the graph, Build a dictionary to match the city name to the number of nodes
    printf("Enter the number of cities on the distribution network: ");
    scanf("%d", &num);
    Graph graph = newGraph(num);

    while(i < num) {
        scanf("%32s", temp_city_node.city_name);
        temp_city_node.node_num = i;
        dictionary[i] = temp_city_node;
        i = i + 1;
    }

    for(i = 0; i < num; i++) {
        for(j=i+1; j < num; j++)
        {
            if(strcmp(dictionary[i].city_name, dictionary[j].city_name)>0)
            {
                temp_city_node = dictionary[i];
                dictionary[i] = dictionary[j];
                dictionary[j] = temp_city_node;
            }
        }
    }


    printf("Enter the number of roads: ");
    scanf("%d", &road_num);

    for(i = 0; i < road_num; i++){
        printf("Enter the name of a city: ");
        scanf("%32s", start_city);
        for(j = 0; j < num; j++){
            if (strcmp(dictionary[j].city_name, start_city) == 0){
                edge.v = dictionary[j].node_num;
                break;
            }
        }

        printf("Enter the name of a city: ");
        scanf("%32s", end_city);
        for(j = 0; j < num; j++){
            if (strcmp(dictionary[j].city_name, end_city) == 0){
                edge.w = dictionary[j].node_num;
                break;
            }
        }

        printf("Enter the distance: ");
        scanf("%d", &weight);
        edge.weight = weight;
        insertEdge(graph, edge);

    }

    printf("\nEnter the required maximum distance: ");
    scanf("%d", &max_distance);



    for(i = 0; i < num; i++){
        getInformation(graph, dictionary[i].node_num, i);
    }

    for(i = 1;i <= num; i++){
        pointer = 0;
        findHubs(max_distance, num, i, graph);
        if(continue_yes_or_no == 0){
            break;
        }
    }
//        for(i = 0; i<num;i++){
//        printf("%s:\n", dictionary[i].city_name);
//        for(j = 0;j<num;j++){
//            printf("%s   %d\n", dictionary[j].city_name, city_to_city_min[i][j]);
//        }
//    }
    freeGraph(graph);

}
