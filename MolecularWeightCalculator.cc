/*************************************************************************
  > File Name: MolecularWeightCalculator.cc
  > Author: luliang
  > Mail: lulyon@126.com 
  > Created Time: 2013年06月21日 星期五 20时20分32秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <algorithm>

const int weightTableLength = 250;
const int queryLength = 550;

typedef struct {
  char name[5];
	double weight;
}WeightTable;

typedef struct {
	char name[5];
	double quantity;
	int index;
}QuantityTable;

bool isUpper(char c)
{
	if(c >= 'A' && c <= 'Z') return true;
	return false;
}

bool isLower(char c) 
{
	if(c >= 'a' && c <= 'z') return true;
	return false;
}

bool isDigit(char c) 
{
	if(c >= '0' && c <= '9') return true;
	return false;
}

void solve(char *query, int left, int right, QuantityTable *qtable, int *top, double times) {
	if(left > right) return;

	int &qtabletop = *top;
	if(isUpper(query[left])) {
		qtable[qtabletop].name[0] = query[left];
		++left;
		if(left <= right && isLower(query[left])) {
			qtable[qtabletop].name[1] = query[left];
			qtable[qtabletop].name[2] = 0;

			++left;
		}
		else {
			qtable[qtabletop].name[1] = 0;
		}

		if(isDigit(query[left])) {
			qtable[qtabletop].quantity = (query[left] - '0') * times;
			++left;
		}
		else {
			qtable[qtabletop].quantity = times;
		}
		++qtabletop;

		QuantityTable *qtable2 = (QuantityTable *)malloc(queryLength * sizeof(QuantityTable));
		int qtable2top = 0;
		solve(query, left, right, qtable2, &qtable2top, times);
		for(int i = 0; i < qtable2top; ++i) {
			int flag = 1;
			for(int j = 0; j < qtabletop; ++j) {
				if(strcmp(qtable2[i].name, qtable[j].name) == 0) {
					qtable[j].quantity += qtable2[i].quantity;
					flag = 0;
					break;
				}
			}
			if(flag) {
				strcpy(qtable[qtabletop].name, qtable2[i].name);
				qtable[qtabletop].quantity = qtable2[i].quantity;
				++qtabletop;
			}
		}

		free(qtable2);
	}
	else if(query[left] == '(') {
		int bracketcount = 1;
		int index = left + 1;
		while(index <= right) {
			if(query[index] == '(') bracketcount++;
			else if(query[index] == ')') bracketcount--;

			if(bracketcount == 0) break;
			++index;
		}

		int index2 = index + 1;
		double lefttimes;
		if(index2 <= right && isDigit(query[index2])) {
			lefttimes = times * (query[index2] - '0');
			++index2;
		}
		else {
			lefttimes = times;
		}
		solve(query, left + 1, index - 1, qtable, &qtabletop, lefttimes);

		QuantityTable *qtable2 = (QuantityTable *)malloc(queryLength * sizeof(QuantityTable));
		int qtable2top = 0;
		solve(query, index2, right, qtable2, &qtable2top, times);
		for(int i = 0; i < qtable2top; ++i) {
			int flag = 1;
			for(int j = 0; j < qtabletop; ++j) {
				if(strcmp(qtable2[i].name, qtable[j].name) == 0) {
					qtable[j].quantity += qtable2[i].quantity;
					flag = 0;
					break;
				}
			}
			if(flag) {
				strcpy(qtable[qtabletop].name, qtable2[i].name);
				qtable[qtabletop].quantity = qtable2[i].quantity;
				++qtabletop;
			}
		}

		free(qtable2);

	}
}

bool cmp(const QuantityTable &a, const QuantityTable &b) {
	if(a.index < b.index) return true;
	return false;
}

int main()
{
	int t;
	scanf("%d", &t);
	while(t--) {
		int n;
		scanf("%d", &n);
		WeightTable wtable[weightTableLength];
		for(int i = 0; i < n; ++i) {
			scanf("%s %lf", wtable[i].name, &wtable[i].weight);
		}
		int m;
		scanf("%d", &m);
		char query[queryLength];
		while(m--) {
			scanf("%s", query);
			QuantityTable *qtable = (QuantityTable *)malloc(queryLength * sizeof(QuantityTable));
			int top = 0;
			solve(query, 0, strlen(query) - 1, qtable, &top, 1);
			double weight = 0;
			for(int i = 0; i < top; ++i) {
				for(int j = 0; j < n; ++j) {
					if(strcmp(qtable[i].name, wtable[j].name) == 0) {
						weight += wtable[j].weight * qtable[i].quantity;
						qtable[i].index = j;
						break;
					}
				}
			}
			std::sort(qtable, qtable + top, cmp);
			printf("%.2lf", weight);
			for(int i = 0; i < top; ++i) {
				printf(" %s[%.0lf]", qtable[i].name, qtable[i].quantity);
			}
			printf("\n");

			free(qtable);
		}
	}

	return 0;
}
