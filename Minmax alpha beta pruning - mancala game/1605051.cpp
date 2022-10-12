#include<bits/stdc++.h>
using namespace std;

vector<int> initialState;
int w1 = 5, w2 = 3, w3 = 2, DEPTH = 5;
bool isSecondPlayer;
int inf = 1000000000;

int minValue(vector<int> &state, int depth, int alpha, int beta, int startIdx, int heuristic, int additionalMove);
int maxValue(vector<int> &state, int depth, int alpha, int beta, int startIdx, int heuristic, int additionalMove);

void horizontalLine()
{
    cout <<"+";
    for (int i = 0; i < 7; i++)
    {
        cout <<"----+";
    }
    cout << "----+" << endl;
}

void showGameState(vector<int> &gameState)
{
    horizontalLine();
    cout << "|  F | ";

    for (int i = 5; i >= 0; i--)
    {
        cout << setw(2) << gameState[i] << " | ";
    }
    cout << " S |" << endl;

    horizontalLine();

    cout << "| " << setw(2) << gameState[12] << " | ";

    for (int i = 6; i < 12; i++)
    {
        cout << setw(2) << gameState[i] << " | ";
    }

    cout << setw(2) << gameState[13] << " | " << endl;
    horizontalLine();
}

void initializeGame(vector<int> &state)
{
    state.assign(14, 0);

    for (int i = 0; i < 12; i++) state[i] = 4;
}

int getHeuristic(vector<int> &state, int heuristic, int additionalMove = 0)
{
    int myStorage = state[12];
    int opponentStorage = state[13];
    int mySide = 0, opponentSide = 0;

    for (int i = 0; i < 6; i++) mySide += state[i];

    for (int i = 6; i < 12; i++) opponentSide += state[i];

    if (isSecondPlayer)
    {
        swap(mySide, opponentSide);
        swap(myStorage, opponentStorage);
    }

    if (heuristic == 1)
    {
        return myStorage - opponentStorage;
    }
    else if (heuristic == 2)
    {
        return w1 * (myStorage - opponentStorage) + w2 * (mySide - opponentSide);
    }
    else if (heuristic == 3)
    {
       return w1 * (myStorage - opponentStorage) + w2 * (mySide - opponentSide) + w3 * additionalMove;
    }
    else if (heuristic == 4)
    {
        int myCnt = 0, opponentCnt = 0, from = 5, howMany = 1, cnt2 = 0;

        for (int i = 0; i < 6; i++)
        {
            if (state[from - i] == howMany) myCnt++;
            howMany++;
        }

        from = 11;
        howMany = 1;

        for (int i = 0; i < 6; i++)
        {
            if (state[from - i] == howMany) opponentCnt++;
            howMany++;
        }

        if (isSecondPlayer) swap(myCnt, opponentCnt);

        return w1 * (myStorage - opponentStorage) + w2 * (mySide - opponentSide) + w3 * additionalMove + (myCnt - opponentCnt) * w3;
    }
    else if (heuristic == 5)
    {
        int closeMySide = 0, closeOpponentSide = 0;

        for (int i = 0; i < 6; i++)
        {
            int cntt = state[i];
            int j = i + 1;

            while (cntt)
            {
                if (j == 6)
                {
                    cntt--;
                    closeMySide++;
                    if (cntt) {
                        cntt--;
                        j++;
                    }
                    continue;
                }
                if (j < 6) closeMySide++;

                cntt--;
                j++;
                j %= 12;
            }
        }

        for (int i = 6; i < 12; i++)
        {
            int cntt = state[i];
            int j = i + 1;
            j %= 12;

            while (cntt)
            {
                if (j == 0)
                {
                    cntt--;
                    closeOpponentSide++;
                    if (cntt) {
                        cntt--;
                        j++;
                        j %= 12;
                    }
                    continue;
                }
                if (j > 5) closeOpponentSide++;

                cntt--;
                j++;
                j %= 12;
            }
        }

        if (isSecondPlayer) swap(closeMySide, closeOpponentSide);

        return w1 * (myStorage - opponentStorage) + w2 * (mySide - opponentSide) + w3 * additionalMove + (closeMySide - closeOpponentSide) * w2;
    }
    else if (heuristic == 6)
    {
        int closeMySide = 0, closeOpponentSide = 0;

        for (int i = 0; i < 6; i++)
        {
            int cntt = state[i];
            int j = i + 1;

            while (cntt)
            {
                if (j == 6)
                {
                    cntt--;
                    closeMySide++;
                    if (cntt) {
                        cntt--;
                        j++;
                    }
                    continue;
                }
                if (j < 6) closeMySide++;

                cntt--;
                j++;
                j %= 12;
            }
        }

        for (int i = 6; i < 12; i++)
        {
            int cntt = state[i];
            int j = i + 1;
            j %= 12;

            while (cntt)
            {
                if (j == 0)
                {
                    cntt--;
                    closeOpponentSide++;
                    if (cntt) {
                        cntt--;
                        j++;
                        j %= 12;
                    }
                    continue;
                }
                if (j > 5) closeOpponentSide++;

                cntt--;
                j++;
                j %= 12;
            }
        }

        if (isSecondPlayer) swap(closeMySide, closeOpponentSide);

        return w1 * (myStorage - opponentStorage) + w3 * additionalMove + (closeMySide - closeOpponentSide) * w2;
    }
}

bool isFinalState(vector<int> &state)
{
    int cnt = 0;

    for (int i = 0; i < 6; i++) cnt += state[i];

    if (cnt == 0) return true;

    cnt = 0;

    for (int i = 6; i < 12; i++) cnt += state[i];

    return (cnt == 0);
}

pair<bool, vector<int> > makeMove(vector<int> state, int idx)///return <isAdditional, nextmove>
{

    if (state[idx] == 0) return {false, state};

    bool firstPlayer = true;
    if (idx > 5) firstPlayer = false;

    int cnt = state[idx];
    state[idx] = 0;
    bool isAdditional = false;
    idx = (idx + 1) % 12;

    while (cnt)
    {
        if (firstPlayer && idx == 6 && isAdditional == false)
        {
            state[12]++;
            cnt--;
            isAdditional = true;

            continue;
        }
        if (!firstPlayer && idx == 0 && isAdditional == false)
        {
            state[13]++;
            cnt--;
            isAdditional = true;
            continue;
        }

        isAdditional = false;
        cnt--;



        state[idx]++;

        if (firstPlayer && idx < 6 && state[idx] == 1 && state[11 - idx] != 0 && cnt == 0)
        {
            state[12] += state[11 - idx] + 1;
            state[11 - idx] = 0;
            state[idx] = 0;
        }

        if (!firstPlayer && idx >= 6 && state[idx] == 1 && state[11 - idx] != 0 && cnt == 0)
        {
            state[13] += state[11 - idx] + 1;
            state[11 - idx] = 0;
            state[idx] = 0;
        }

        idx++;
        idx %= 12;
    }

    return {isAdditional, state};
}

vector<int> getRandomOrder()
{
    vector<int> indices;
    map<int,int> taken;

    while (indices.size() < 6)
    {
        int now = (abs(rand())) % 6;
        if (!taken[now])
        {
            taken[now] = 1;
            indices.push_back(now);
        }
    }
    return indices;
}

int maxValue(vector<int> &state, int depth, int alpha, int beta, int startIdx, int heuristic, int additionalMove)
{
    if (depth == 0 || isFinalState(state))
    {
        return getHeuristic(state, heuristic, additionalMove);
    }
    int v = -inf;

    vector<int> indices = getRandomOrder();

    for (int ii = 0; ii < 6; ii++)
    {
        int i = indices[ii];

        if (state[i + startIdx] > 0)
        {
            pair<bool,vector<int> > tempState = makeMove(state, i + startIdx);
            int res;
            if (tempState.first == true)
            {
                res = maxValue(tempState.second, depth - 1, alpha, beta, startIdx, heuristic, additionalMove + 1);
            }
            else res = minValue(tempState.second, depth - 1, alpha, beta, (startIdx + 6) % 12, heuristic, additionalMove);

            v = max(v, res);

            if (v >= beta) return v;
            alpha = max(alpha, v);

        }
    }

    return v;

}
int minValue(vector<int> &state, int depth, int alpha, int beta, int startIdx, int heuristic, int additionalMove)
{
    if (depth == 0 || isFinalState(state))
    {
        return getHeuristic(state, heuristic, additionalMove);
    }
    int v = inf;

    vector<int> indices = getRandomOrder();

    for (int ii = 0; ii < 6; ii++)
    {
        int i = indices[ii];

        if (state[i + startIdx] > 0)
        {
            pair<bool,vector<int> > tempState = makeMove(state, i + startIdx);
            int res;
            if (tempState.first == true)
            {
                res = minValue(tempState.second, depth - 1, alpha, beta, startIdx, heuristic, additionalMove);
            }
            else res = maxValue(tempState.second, depth - 1, alpha, beta, (startIdx + 6) % 12, heuristic, additionalMove);

            v = min(v, res);

            if (v <= alpha) return v;
            beta = min(beta, v);

        }
    }

    return v;
}

pair<bool, vector<int> > alphaBetaSearch(vector<int> &state, int startIdx, int heuristic)
{
    pair<bool, vector<int> > nextstate = {false, state};
    int v = -inf;
    int alpha = -inf, beta = inf;

    isSecondPlayer = (startIdx == 6);

    vector<int> indices = getRandomOrder();

    for (int ii = 0; ii < 6; ii++)
    {
        int i = indices[ii];
        if (state[i + startIdx] > 0)
        {

            pair< bool, vector<int> > tempState = makeMove(state, i + startIdx);
            int res;
            if (tempState.first == true)
            {
                res = maxValue(tempState.second, DEPTH - 1, alpha, beta, startIdx, heuristic, 1);
            }
            else res = minValue(tempState.second, DEPTH - 1, alpha, beta, (startIdx + 6) % 12, heuristic, 0);

            if (res > v) nextstate = tempState;

            v = max(v, res);

            if (v >= beta) return nextstate;
            alpha = max(alpha, v);
        }
    }

    return nextstate;
}

void playAIvsAI(vector<int> state, int heuristic)
{
    vector<int> tempState = state;
    int win = 0;
    for (int ii = 0; ii < 100; ii++)
    {
        ///cout << ii << endl;

        state = tempState;
        while (!isFinalState(state))
        {
            while (!isFinalState(state))
            {
                pair<bool, vector<int> > nextState = alphaBetaSearch(state, 0, heuristic);
                state = nextState.second;
                if (!nextState.first) break;
            }
            while (!isFinalState(state))
            {
                pair<bool, vector<int> > nextState = alphaBetaSearch(state, 6, heuristic);
                state = nextState.second;
                if (!nextState.first) break;
            }
        }

        int cntFirst = state[12];
        for (int i = 0; i < 6; i++) cntFirst += state[i];

        if (cntFirst > 24) win++;
    }

    cout << win << " " << 100 - win << endl;
}

void playAIvsUser(vector<int> state, int heuristic)
{
    showGameState(state);
    cout << "Do you want to play first? (y/n): ";
    char ch;

    cin >> ch;
    int myIdx = 0;
    int aiIdx = 6;

    if (ch != 'y')
    {
        myIdx = 6;
        aiIdx = 0;
        while(true)
        {
            pair<bool, vector<int> > nextState = alphaBetaSearch(state, 0, heuristic);
            cout << "ai moved" << endl;
            showGameState(nextState.second);

            state = nextState.second;
            if (!nextState.first) break;
        }

    }

    while (!isFinalState(state))
    {
        while (!isFinalState(state))
        {
            cout << "your move. enter a number between 1 to 6. 6 is closer to storage: ";
            int mov;
            cin >> mov;
            mov--;
            mov += myIdx;

            if (mov < 0 || mov >= 12 || state[mov] == 0)
            {
                cout << "Invalid bin" << endl;
                continue;
            }
            pair<bool, vector<int> > tempState = makeMove(state, mov);
            showGameState(tempState.second);
            state = tempState.second;

            if (!tempState.first) break;
        }

        while (!isFinalState(state))
        {
            pair<bool, vector<int> > nextState = alphaBetaSearch(state, aiIdx, heuristic);
            cout << "ai moved" << endl;
            showGameState(nextState.second);
            state = nextState.second;
            if (!nextState.first) break;
        }
    }
}

int main()
{
    vector<int> state;
    initializeGame(state);

    playAIvsAI(state, 1);
    playAIvsAI(state, 2);
    playAIvsAI(state, 3);
    playAIvsAI(state, 4);
    playAIvsAI(state, 5);
    playAIvsAI(state, 6);

    playAIvsUser(state, 3);

}

