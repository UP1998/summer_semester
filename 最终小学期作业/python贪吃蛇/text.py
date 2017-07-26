#使用模块
import pygame #使用pygame写游戏
import sys    #包含了与Python解释器和它的环境有关的函数。是system的缩写
import random #包含随机功能的模块
import pyglet
pygame.init()
pygame.mixer.init()
sound = pygame.mixer.Sound("music.wav")
sound.play()


# 全局定义
SCREEN_X = 600 #横向
SCREEN_Y = 600 #竖向

def real_playmusic():
    sound = pyglet.resource.media("music.mp3")
    sound.play()
    pyglet.app.run()

def playmusic():
    global player_thread
    player_thread = Thread(target=real_playmusic)
    player_thread.start()

def stopsound():
    pyglet.app.exit

# 蛇类
class Snake(object):
    # 初始化各种需要的属性
    def __init__(self):
        self.dirction = pygame.K_RIGHT #开始游戏的时候自动设置方向向右
        self.body = [] #初始化一个列表，self.body是列表
        for x in range(5): #for循环 初始化5个结点
            self.addnode() #调用addnode函数，增加结点

    # 无论何时 都在前端增加蛇块
    def addnode(self):
        left, top = (0, 0)   #解包，left为0，top为0
        if self.body:  #如果self.body存在
            left, top = (self.body[0].left, self.body[0].top) #解包，left为self.body[0].left；top为self.body[0].top
        node = pygame.Rect(left, top, 25, 25) #结点为pygame中有left和top属性的长宽为25的正方形
        if self.dirction == pygame.K_LEFT: #若此时方向为左，则node.left减去一个结点的长度
            node.left -= 25
        elif self.dirction == pygame.K_RIGHT:
            node.left += 25
        elif self.dirction == pygame.K_UP:
            node.top -= 25
        elif self.dirction == pygame.K_DOWN:
            node.top += 25
        self.body.insert(0, node)  #把node插入到0的位置，也即列表头

    # 删除最后一个块
    def delnode(self):
        self.body.pop() #返回的是最后一个的地址，并且将它从列表中删除。没指定[i]

    # 死亡判断
    def isdead(self):
        # 撞墙
        if self.body[0].x not in range(SCREEN_X): #出界就死了
            return True
        if self.body[0].y not in range(SCREEN_Y):
            return True
        # 撞自己
        if self.body[0] in self.body[1:]: #列表第一个在第二个到最后一个中间
            return True
        return False

    # 移动！
    def move(self): #动态动，前面加一个，后面删一个
        self.addnode() #加一个点
        self.delnode() #删一个点

    # 改变方向 但是左右、上下不能被逆向改变
    def chdirection(self, head):
        LR = [pygame.K_LEFT, pygame.K_RIGHT]
        UD = [pygame.K_UP, pygame.K_DOWN]
        if head in LR + UD:
            if (head in LR) and (self.dirction in LR): #如果头和方向都在一起，则无作为
                return #返回None 退出函数
            if (head in UD) and (self.dirction in UD):
                return
            self.dirction = head #如果头和方向不在一起，则转换方向


# 食物类
class Food:
    def __init__(self):
        self.rect = pygame.Rect(-25, 0, 25, 25) #Rect(left,top,width,height)

    def remove(self):
        self.rect.x = -25

    def set(self):
        if self.rect.x == -25:
            allpos = []
            # 不靠墙太近 25 ~ SCREEN_X-25 之间
            for pos in range(25, SCREEN_X - 25, 25):
                allpos.append(pos)
            self.rect.left = random.choice(allpos)
            self.rect.top = random.choice(allpos)
            print(self.rect)


def show_text(screen, pos, text, color, font_bold=False, font_size=60, font_italic=False):
    # 获取系统字体，并设置文字大小
    cur_font = pygame.font.SysFont("Calibri", font_size)
    # 设置是否加粗属性
    cur_font.set_bold(font_bold)
    # 设置是否斜体属性
    cur_font.set_italic(font_italic)
    # 设置文字内容
    text_fmt = cur_font.render(text, 1, color)
    # 绘制文字
    screen.blit(text_fmt, pos)

def main():

    screen_size = (SCREEN_X, SCREEN_Y)
    screen = pygame.display.set_mode(screen_size)
    pygame.display.set_caption('Snake Game')
    clock = pygame.time.Clock()
    scores = 0
    time = 0
    isdead = False

    # 蛇/食物
    snake = Snake()
    food = Food()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            if event.type == pygame.KEYDOWN:
                snake.chdirection(event.key)
                # 死后按space重新开始调用main函数
                if event.key == pygame.K_SPACE and isdead:
                    return main()
                #死后按enter结束游戏进程
                if event.key == pygame.K_RETURN and isdead:
                    exit(0)

        screen.fill((0, 0, 0))

        # 画蛇身 / 每一步+1分
        if not isdead:
            time += 1
            snake.move()
        for rect in snake.body:
            pygame.draw.rect(screen, (255, 0, 255), rect, 0)

        # 显示死亡文字
        isdead = snake.isdead()
        if isdead:
            show_text(screen, (40, 200), 'SORRY,DEAD!', (227, 29, 18), False, 100)
            show_text(screen, (150, 280), 'press space to try again...', (150, 150, 150), False, 30)
            show_text(screen, (150, 310), 'press enter to exit...', (150, 150, 150), False, 30)

        # 食物处理 / 吃到+50分
        # 当食物rect与蛇头重合,吃掉 -> Snake增加一个Node
        if food.rect == snake.body[0]:
            scores += 1
            food.remove()
            snake.addnode()

        # 食物投递
        food.set()
        pygame.draw.rect(screen, (136, 0, 21), food.rect, 0)

        # 显示分数文字
        show_text(screen, (50, 500), 'Scores: ' + str(scores), (223, 223, 223))
        show_text(screen, (300, 500), 'Time: ' + str(time), (223, 223, 223))

        pygame.display.update()
        clock.tick(10)#行进速度

#写的脚本模块既可以导入到别的模块中用，另外该模块自己也可执行。
if __name__ == '__main__':
    main()