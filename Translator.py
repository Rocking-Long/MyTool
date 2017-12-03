#作者:Rocking1996
#日期:2017.10

 
#-*- coding:utf-8 -*-
import requests
import bs4
import os

wordType = ['v', 'n', 'vt', 'vi', 'adj', 'adv', 'int', 'prep', 'conj', 'pron', 'num', 'art']
lan = ['zh' , 'en', 'jp', 'fra', 'de', 'it','wyw']
lanc = ['中文', '英文', '日文', '法文', '德文', '意大利文', '文言文']
L = 'en'
history = ''
openSave = False
pre = ''

def help():
    '''
    获得帮助，为纯文本
    '''
    print('******************************')
    print('***   Little Translator    ***')
    print('******************************')
    print(' ')
    print('本程序可执行%d门语言的翻译'%len(lan))
    print('具体操作如下--')
    print('**输入英文单词，查询完整单词释义')
    print('  如：>>>English')
    print('**输入英文句子，可查询中文翻译')
    print('  如：>>>today is a beautiful day')
    print('**输入中文句子，可查询英文翻译')
    print('  如：>>>我后天回家')
    print('**输入语言标识，可在两门语言间翻译')
    print('  如：<zh,jp>今天你可真漂亮')
    print('**输入语言标识，可翻译成中文')
    print('  如：<jp>今日はいいお天気ですね')
    print(' ')
    print('可翻译语言如下--')
    for i in range(len(lan)):
        print('%-4s%s'%(lan[i], lanc[i]))
    print(' ')
    print('可用指令--')
    print('-config     语言配置——选择与中文互译的语言')
    print('-save          保存记录——将记录保存到文件中')
    print('-open       打开保存开关——打开后可保存之后的数据')
    print('-close      关闭保存开关——关闭后不保存之后的数据')
    print('-ap         添加前缀——可在为输入添加前缀')
    print('-dp         删除前缀——删除前缀')
    print('-c          查看当前设置')
    print('-exit       退出')
    print('\n')


def config():
    '''
    进行语言配置
    选择与中文互译的语言
    '''
    print('可配置语言如下--')
    for i in range(1, len(lan)):
        print('%-4s%s'%(lan[i], lanc[i]))
    while True:
        op = input("请选择与中文互译的语言(默认en)：")
        if not op in lan[1:]:
            print(op, '不在可翻译列表中')
            continue
        else:
            break

    global L
    L = op
    print('\n')
    
    
    
def check():
    '''
    查看当前设置
    '''
    print('当前选择语言:', L)
    if openSave:
        print('保存开关:', '开')
    else:
        print('保存开关:', '关')
    if len(pre) == 0:
        print('前缀:', '无')
    else:
        print('前缀:', pre)
    print(' ')


def translate(word):
    """
    有道翻译
    用于翻译单词
    """
    global history
    global openSave
    rooturl = 'http://www.youdao.com/w/'
    finurl = rooturl + word
    try:
        response = requests.get(finurl)
        soup = bs4.BeautifulSoup(response.text,'html.parser')
        tran = soup.select('.trans-container > ul > li')
        for i in tran:
            i = i.get_text()
            if (i[:1] in wordType) or \
               (i[:2] in wordType) or \
               (i[:3] in wordType) or \
               (i[:4] in wordType):
                if openSave:
                    history += i + '\n'
                print(i)
            else:
                if openSave:
                    history += '\n'
                break

    except IndexError:
        print("没找到这个词")


def fanyi(context, language):
    """
    百度翻译
    用于翻译句子和短语
    """
    global history
    global openSave
    url = 'http://fanyi.baidu.com/v2transapi/'
    data = {
        'from':language[0],
        'to':language[1],
        'query':context,
        'transtype':'translang',
        'simple_means_flag':'3',
    }
    headers ={'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.153 Safari/537.36 SE 2.X MetaSr 1.0'}
    response = requests.post(url,data,headers=headers)
    res = response.json()['trans_result']['data'][0]['dst']
    
    if openSave:
        history += res + '\n\n'
    print(res)


def isChinese(uchar):
    '''
    判断是否是中文
    '''
    if u'\u4e00' <= uchar <= u'\u9fff':
        return True
    return False

def judge(text):
    '''
    进行文本解析
    获取文本翻译信息
    决定如何翻译该文本
    '''
    #<x x>型
    if text[0] == '<':
        language1 = ''
        language2 = ''
        i = 0
        #language1
        while not text[i].isalpha():
            i += 1
        while text[i].isalpha():
            language1 += text[i]
            i += 1
        if not language1 in lan:
            print(language1 + '不在可翻译列表中')
            return None
        #<lan>型
        j = i
        while not text[j].isalpha():
            if text[j] == '>':
                j += 1
                while text[j].isdigit():
                    j += 1
                return [text[j:], [language1, 'zh']]
            j += 1
        #<lan1,lan2>型
        #language2
        while not text[i].isalpha():
            i += 1
        while text[i].isalpha():
            language2 += text[i]
            i += 1
        #<lan1,lan2>型
        if not language2 in lan:
            print(language2 + '不在可翻译列表中')
            return None
        while text[i] != '>':
            i += 1
        i += 1
        while text[i].isdigit():
            i += 1
        return [text[i:], [language1, language2]]

    #默认型
    i = 0
    while text[i].isdigit():
        i += 1
    #判断是否是英文单词
    isWord = False
    if text[i:].encode('UTF-8').isalpha():
        isWord = True
    #翻译完整单词
    if isWord:
        return (text, 'w')
    #与中文互译翻译
    if isChinese(text[i]):
        return (text, ('zh', L))
    return (text, (L, 'zh'))





def saveFlie():
    '''
    保存记录
    创建一个文件
    清空hostory
    '''
    while True:
        name = input("请输入文件名(输入-no放弃保存)：")
        if name == '-no':
            return
        if os.path.isfile(name) == True:
            print("文件存在，请重新输入")
            continue
        break
    fp = open(name, 'w')
    global history
    fp.write(history)
    fp.close()
    print('保存完毕')
    history = '' #清空history




if __name__ == '__main__':
    # global history
    print("输入-help取得帮助")
    while True:
        # w = raw_input(">>>")
        w = input('>>>')
        w = w.strip()
        if w == '':
            continue 

        #指令判断
        if w[0] == '-':
            if w == '-help':
                help()
            if w == '-config':
                config()
            if w == '-save':
                saveFlie()
            if w == '-open':
                openSave = True
                print('--save switch has opened')
            if w == '-close':
                openSave = False
                print('--save switch has closed')
            if w == '-ap':
                pre = input("请输入前缀：")
            if w == '-dp':
                pre = ''
            if w == '-c':
                check()
            if w == '-exit':
                break
            continue

        #语言翻译
        w = pre + w
        language = judge(w)
        # print(language)
        if language == None:
            print(' ')
            continue
        elif language[1] == 'w':
            if openSave:
                history += w + '\n'
            translate(language[0])
        else:
            #两种语言一样会报错
            if openSave:
                history += w + '\n'
            if language[1][0] == language[1][1]:
                print('两种语言不可一样，已改成中文-英文')
                language[1][0] = 'zh'
                language[1][1] = 'en'
            fanyi(language[0], language[1])
        print(' ')
        # print(history)

    print('已退出Translator')
