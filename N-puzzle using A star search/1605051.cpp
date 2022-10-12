#include <bits/stdc++.h>
using namespace std;


#define debug(...)                    cerr << __LINE__ << " : (" << #__VA_ARGS__ << ") = ("; _Print(__VA_ARGS__);
template<class T> void _Print(T &&x) { cerr << x << ")" << endl; }
template<class T, class ...S> void _Print(T &&x, S &&...y) { cerr << x << ", "; _Print(y...); }


int dirx[] = {-1, 1, 0, 0};
int diry[] = {0, 0, -1, 1};

int n, k;
vector<int> initialGrid;
vector<int> finalGrid;

#define humming 1
#define manhattan 2
#define linear_conflict 3

int calcDistance(vector<int> &grid, int heuristicFunction)
{
    int distance = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (grid[i] != 0 && grid[i] != finalGrid[i])
        {
            distance++;
        }
    }

    if (heuristicFunction == humming) return distance;

    distance = 0;

    for (int i = 0; i < n * n; i++)
    {
        if (finalGrid[i] != 0)
        {
            int rowf = i / n, colf = i % n;

            for (int j = 0; j < n * n; j++)
            {
                if (grid[j] == finalGrid[i])
                {
                    int row = j / n, col = j % n;

                    distance += abs(rowf - row) + abs(colf - col);
                    break;
                }
            }

        }
    }

    if (heuristicFunction == manhattan) return distance;

    int targetrow[n * n];
    int targetColumn[n + n];

    for (int i = 0; i < n * n; i++)
    {
        if (finalGrid[i] != 0)
        {
            targetrow[finalGrid[i]] = i / n;
            targetColumn[finalGrid[i]] = i % n;
        }
    }

    vector<vector<int> > rowData(n);///keeps the target columns in respective row

    for (int i = 0; i < n * n; i++)
    {
        if (grid[i] != 0 && targetrow[grid[i]] == i / n)
        {
            //debug(grid[i]);
            rowData[i / n].push_back(targetColumn[grid[i]]);
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < rowData[i].size(); j++)
        {
            for (int m = j + 1; m < rowData[i].size(); m++)
            {
                if (rowData[i][j] > rowData[i][m]) distance += 2;
            }
        }
    }

    if (heuristicFunction == linear_conflict) return distance;
    return 0;

}

void showCostAndSteps(int steps, map<int,int> &parenNode, map<int, vector<int> > &getNode, map<vector<int> , int> &mapNode)
{
    cout << "Number of steps: " << steps << endl;
    cout << "Steps: " << endl;
    vector<vector<int> > answer;

    int nodeIdx = mapNode[finalGrid];

    while (nodeIdx != -1)
    {
        answer.push_back(getNode[nodeIdx]);
        nodeIdx = parenNode[nodeIdx];
    }

    reverse(answer.begin(), answer.end());

    for (int i = 0; i < answer.size(); i++)
    {
        for (int j = 0; j < n * n; j++)
        {
            if (answer[i][j] == 0)
            {
                cout << " *";
            }
            else
            {
                cout << " " << answer[i][j];
            }

            if (j % n == n - 1) cout << endl;
        }
        cout << endl;
    }

}

void A_star_search(int heuristicFunction)
{
    priority_queue<pair<int,pair<int,int> >, vector<pair<int,pair<int,int>>>, greater<> > nodes;//f(n)-g(n)-node
    map<int, vector<int> > getNode;
    map<vector<int>, int> mapNode;
    map<int, int> minNodeCost;//min f(n) for a node
    map<int, int> parentNode;

    set<int> closedNodes;

    int nodeCount = 1;

    mapNode[initialGrid] = nodeCount;
    getNode[nodeCount] = initialGrid;
    minNodeCost[nodeCount] = calcDistance(initialGrid, heuristicFunction);
    parentNode[nodeCount] = -1;

    nodes.push({calcDistance(initialGrid, heuristicFunction), {0, nodeCount}});
    nodeCount++;

    while (!nodes.empty())
    {
        auto top = nodes.top();
        nodes.pop();
        //debug(top.first);

        closedNodes.insert(top.second.second);

        if (calcDistance(getNode[top.second.second], heuristicFunction) == 0)
        {
            showCostAndSteps(top.second.first, parentNode, getNode, mapNode);
            cout << "Number of explored nodes: " << nodeCount - 1 << endl;
            cout << "Number of expanded nodes: " << (int) closedNodes.size() - 1 << endl;
            break;
        }

        vector<int> curNode = getNode[top.second.second];
        int blankIndex = 0;

        for (int i = 0; i < n * n; i++)
        {
            if (curNode[i] == 0)
            {
                blankIndex = i;
                break;
            }
        }

        int row = blankIndex / n, col = blankIndex % n;

        for (int i = 0; i < 4; i++)
        {
            if (row + dirx[i] >= 0 && row + dirx[i] < n
            && col + diry[i] < n && col + diry[i] >= 0)
            {
                int r = row + dirx[i], c = col + diry[i];

                vector<int> nextNode = curNode;
                swap(nextNode[blankIndex], nextNode[r * n + c]);

                if (mapNode.find(nextNode) == mapNode.end())
                {
                    mapNode[nextNode] = ++nodeCount;
                    getNode[nodeCount] = nextNode;
                }

                int nodeIdx = mapNode[nextNode];

                if (closedNodes.find(nodeIdx) != closedNodes.end()) continue;

                int f_n = top.second.first + 1 + calcDistance(nextNode, heuristicFunction);

                if (minNodeCost.find(nodeIdx) == minNodeCost.end() ||
                minNodeCost[nodeIdx] > f_n)
                {
                    minNodeCost[nodeIdx] = f_n;
                    nodes.push({f_n, {top.second.first + 1, nodeIdx}});
                    parentNode[nodeIdx] = top.second.second;
                }
            }
        }
    }
}

void inputGrid(vector<int> &initialGrid, int n)
{
    initialGrid.resize(n * n);
    finalGrid.resize(n * n);

    for (int i = 1; i < n * n; i++)
        finalGrid[i - 1] = i;
    finalGrid[n * n - 1] = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> initialGrid[i * n + j];
        }
    }
}

bool isSolvable(vector<int> &grid, int n)
{
    int inversions = 0, blank = 0;

    for (int i = 0; i < grid.size(); i++)
    {
        if (grid[i] == 0)
        {
            blank = n - (i / n);
        }

        for (int j = i + 1; j < grid.size(); j++)
        {
            if (grid[i] > grid[j] && grid[i] !=0 && grid[j] != 0)
            {
                inversions++;
            }
        }
    }

    if (n & 1)
    {
        return !(bool)(inversions & 1);
    }

    if ((inversions & 1) && !(blank & 1)) return true;
    if (!(inversions & 1) && (blank & 1)) return true;

    return false;
}

int main()
{
    cout << "enter the value of k: ";
    cin >> k;
    cout << "Enter the grid, 0 for blank: " << endl;

    n = k;
    inputGrid(initialGrid, n);

    if (isSolvable(initialGrid, n))
    {
        cout << "Humming distance: " << endl;
        A_star_search(humming);

        cout << "\nManhattan distance: " << endl;
        A_star_search(manhattan);

        cout << "\nLinear Conflict: " << endl;
        A_star_search(linear_conflict);
    }
    else cout << "Not solvable" << endl;

    return 0;
}
