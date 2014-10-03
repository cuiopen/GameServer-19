
#ifndef ASTAR_H_
#define ASTAR_H_

#include "Pos.h"
#include "core/Random.h"

#include <list>
#include <vector>
#include <algorithm>

namespace buf
{

template <int STEP = 1, int RADIUS = 12>
class AStar
{
public:
    // XXX: check block and range
    virtual bool moveable(const Pos& tempPos, const Pos& dst, int radius = RADIUS) const = 0;
    virtual bool move(int direct, int step = STEP) = 0;

protected:
    bool moveToFindPath(const Pos& src, const Pos& dst);
    bool moveTo(const Pos& src, const Pos& dst);
    bool shiftMove(int direct);

private:
    struct PathPoint
    {
        Pos pos;
        int cc;
        PathPoint* father;
    };

    struct PathQueue
    {
        PathPoint* node;
        int cost;

        PathQueue(PathPoint* node, int cost)
        {
            this->node = node;
            this->cost = cost;
        }

        PathQueue(const PathQueue& queue)
        {
            node = queue.node;
            cost = queue.cost;
        }

        PathQueue& operator= (const PathQueue& queue)
        {
            node = queue.node;
            cost = queue.cost;
            return *this;
        }
    };

    typedef std::list<PathQueue> PathQueueHead;
    typedef typename PathQueueHead::iterator iterator;
    typedef typename PathQueueHead::reference reference;

    int distance(const Pos& midPos, const Pos& endPos)
    {
        int distance = abs(midPos.x - endPos.x) + abs(midPos.y - endPos.y);
        return distance;
    }

    void insertQueue(PathQueueHead& queueHead, PathPoint* pPoint, int currentCost)
    {
        PathQueue pNew(pPoint, currentCost);
        if (!queueHead.empty())
        {
            for (iterator it = queueHead.begin(); it != queueHead.end(); ++it)
            {
                if ((*it).cost > currentCost)
                {
                    queueHead.insert(it, pNew);
                    return;
                }
            }
        }
        queueHead.push_back(pNew);
    }

    PathPoint* popQueue(PathQueueHead &queueHead)
    {
        PathPoint* ret = NULL;
        if (!queueHead.empty())
        {
            reference ref = queueHead.front();
            ret = ref.node;
            queueHead.pop_front();
        }
        return ret;
    }
};

template<int STEP, int RADIUS>
bool AStar<STEP, RADIUS>::moveToFindPath(const Pos& src, const Pos& dst)
{
    // pDisMap是以destPos为中心的边长为2 * radius + 1 的正方形
    const int width = (2 * RADIUS + 1);
    const int height = (2 * RADIUS + 1);
    const int maxNum = width * height;
    int pDisMap[maxNum];
    // 把所有路径距离初始化为最大值
    std::fill_n(pDisMap, maxNum, maxNum);

    PathPoint stack[maxNum * 8 + 1];
    PathQueueHead queueHead;

    // 从开始坐标进行计算
    PathPoint* root = &stack[maxNum * 8];
    root->pos = src;
    root->cc = 0;
    root->father = NULL;
    insertQueue(queueHead, root, root->cc + distance(root->pos, dst));

    const PosS adjust[8] =
    {
        { 1 * STEP,  0 * STEP},
        { 0 * STEP, -1 * STEP},
        { 0 * STEP,  1 * STEP},
        {-1 * STEP,  0 * STEP},
        { 1 * STEP, -1 * STEP},
        {-1 * STEP, -1 * STEP},
        {-1 * STEP,  1 * STEP},
        { 1 * STEP,  1 * STEP}
    };

    int count = 0;
    while (count < maxNum)
    {
        root = popQueue(queueHead);
        if (NULL == root)
            return false; // 目标点不可达

        if (root->pos == dst) // 找到到达目的地的路径
            break;

        for (int i = 0; i < 8; ++i) // 分别对周围8个格点进行计算路径
        {
            bool bCanWalk = true;
            Pos tempPos = root->pos;
            tempPos += adjust[i];

            if (moveable(tempPos, dst)) // 对路径进行回溯
            {
                PathPoint* p = root;
                while (p)
                {
                    if (p->pos == tempPos)
                    {
                        // 发现坐标点已经在回溯路径中，不能向前走(检查是否在close table)
                        bCanWalk = false;
                        break;
                    }
                    p = p->father;
                }

                // 如果路径回溯成功，表示这个点是可行走的
                if (bCanWalk)
                {
                    int cost = root->cc + 1;
                    int index = (tempPos.y - dst.y + RADIUS) * width + (tempPos.x - dst.x + RADIUS);
                    if (index >= 0 && index < maxNum && cost < pDisMap[index])
                    {
                        // 这条路径比上次计算的路径还要短，需要加入到最短路径队列中
                        pDisMap[index] = cost;
                        PathPoint* pNewEntry = &stack[count * 8 + i];
                        pNewEntry->pos = tempPos;
                        pNewEntry->cc = cost;
                        pNewEntry->father = root;
                        insertQueue(queueHead, pNewEntry, pNewEntry->cc + distance(pNewEntry->pos, dst));
                    }
                }
            }
        }

        ++count;
    }

    if (count < maxNum)
    {
        // 最终路径在PointHead中,但只走一步
        while (root)
        {
            if (root->father != NULL && root->father->father == NULL)
            {
                // 倒数第二个节点
                return move(src.dir(root->pos));
            }

            root = root->father;
        }
    }

    return false;
}

template <int STEP, int RADIUS>
inline bool AStar<STEP, RADIUS>::moveTo(const Pos& src, const Pos& dst)
{
    int direct = src.dir(dst);
    if (!move(direct))
    {
        Random rnd;
        int r = rnd(0,1);
        int deep = 0;
        while (deep < 3)
        {
            switch (r)
            {
                case 0:
                    ++direct;
                    break;
                case 1:
                    direct += 7;
                    break;
            }

            if (move(direct % 8))
                return true;
            ++deep;
        }
    }

    return false;
}

template<int STEP, int RADIUS>
inline bool AStar<STEP, RADIUS>::shiftMove(int direct)
{
    return move(direct);
}

} // namespace buf

#endif // ASTAR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

