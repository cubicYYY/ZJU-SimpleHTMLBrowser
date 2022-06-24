/*HTML������Ⱦ��*/
#include <stdio.h>
#include <stdlib.h>

#include "imgui.h"
#include "extgraph.h"

#include "lexerParser.h"
#include "renderEngine.h"
#include "simpleCSS.h"
#include "analysis.h"

//�ӿڱ��� 
string title;//o:�ĵ����� 
float_64 shift_x,shift_y;//i:����������ƫ���� 
bool lazying=FALSE;//i/o:������Ⱦ��־ 
string globalCSS="";//i:ȫ��CSS 
string redirect="";//i/o:�ض��� 

static int now_button=0,tot_href=0;
static string hrefs[MAX_HLINK];//��¼������ť�����ӵ�ַ 

DOM	*dom;
const PRINT_OPT INIT_OPT={.states=0,.color=0,.size=16};
RENDER *rtree;
float_64 rend_x,rend_y;//��Ԫ��x,y��Ϊ��Ԫ��ջ�ṩ֧�֣� 
float_64 area_w,area_h,area_x,area_y;//��Ⱦ����λ����Ϣ 
size_t render_node_count = 0;
RENDER render[MAX_TOKENS];
float_64  renarea_w=0,renarea_h=0;
RENDER *line_obj_stack[MAX_LINE_OBJS];//��Ԫ��ջ 
size_t stack_top = 0;//ջ�� 
float_64 stack_w=0, stack_h=0, stack_align_h=0;//ά���ض��еĸ߶��������Ϣ 

static inline void clearStack();
static inline void addStack(RENDER *node);
string getSubtext(DOM *node)//��ýڵ�Ĵ��ı���ʽ 
{
	string s=RN(node->literal);
	for (size_t i=0;i<node->child_count;i++)
		s=Concat(s,getSubtext(node->childs[i]));
	return s;
}

RENDER *findRenderByID(string id, RENDER *node)//Ѱ��ָ��ID�Ľڵ�
{
	if (FindString(noSpace(id),noSpace(node->id),0)==0) return node;
	for (size_t i=0;i<node->child_count;i++) {
		RENDER *tmp=findRenderByID(id,node->childs[i]);
		if (tmp)
			return tmp;
	}
	return NULL;
}
void jumpByID(string id)//��ת��ָ��ID�ڵ��λ�� 
{
	RENDER *target = findRenderByID(id, rtree);
	if (target) {
		while (target->child_count!=0) target=target->childs[0];
		shift_x=target->x;
		shift_y=-target->y;
	}
}


static RENDER *addRenderNode(float_64 h,float_64 w,float_64 align_h,PRINT_OPT opt,string print_text,string tagtype,string id)//�����Ⱦ���ڵ� 
{
	RENDER *now = &render[render_node_count];
	now->h=h;
	now->w=w;
	now->align_h=align_h;
	now->opt=opt;
	now->print_text=print_text;
	now->tagtype=tagtype;
	now->child_count=0;
	now->id=id;
	++render_node_count;
	return now;
}
PRINT_OPT updateOpt(DOM *root, PRINT_OPT opt) //���ݱ�ǩ��CSS���½ڵ���Ⱦѡ�� 
{	
	//ע�����ȼ�Ϊ����ǩ�Դ�<ȫ��CSS<��ѡ����<IDѡ����<��ǶCSS 
	/*inherent tag style */
	if (StringEqual(root->tagtype,"h1")) opt.size=42,opt.states|=BOLD;
	else if (StringEqual(root->tagtype,"h2")) opt.size=38,opt.states|=BOLD;
	else if (StringEqual(root->tagtype,"h3")) opt.size=34,opt.states|=BOLD;
	else if (StringEqual(root->tagtype,"h4")) opt.size=30,opt.states|=BOLD;
	else if (StringEqual(root->tagtype,"h5")) opt.size=26,opt.states|=BOLD;
	else if (StringEqual(root->tagtype,"h6")) opt.size=22,opt.states|=BOLD;
	else if (StringEqual(root->tagtype, "b")||StringEqual(root->tagtype, "strong")) 
		opt.states|=BOLD;
	else if (StringEqual(root->tagtype, "i")||StringEqual(root->tagtype, "em")||StringEqual(root->tagtype, "strong"))
		opt.states|=ITALIC;
	else if (StringEqual(root->tagtype, "u")||StringEqual(root->tagtype, "ins"))
		opt.states|=UNDERLINE;
	else if (StringEqual(root->tagtype, "del"))
		opt.states|=DELETED;
	else if (StringEqual(root->tagtype, "a")) 
		opt.states|=HYPER;
	else if (StringEqual(root->tagtype, "hr")) 
		opt.states|=HR;
	else if (StringEqual(root->tagtype, "big")) 
		opt.size+=4;
	else if (StringEqual(root->tagtype, "small")) 
		opt.size-=4;
	string classes=ConvertToLowerCase(RN(getAttr(root,"class")));
	string id=ConvertToLowerCase(RN(getAttr(root,"id")));
	opt=updateOptByClass(opt, globalCSS, classes);//��ѡ���� 
	opt=updateOptByID(opt, globalCSS, id);//IDѡ���� 
	opt=updateOptByCSSText(opt, RN(getAttr(root,"style")));//��ǶCSS 
	return opt;
}
void applyPrintOpt(PRINT_OPT opt) //Ӧ�ô�ӡ��Ⱦѡ�� 
{
	int style=((opt.states&ITALIC)?Italic:0);
	style|=((opt.states&BOLD)?Bold:0);
	SetStyle(style);
	SetPointSize(opt.size);
	string color_name=Concat("COLOR_", IntegerToString(opt.color));
	DefineColor(Concat("COLOR_", IntegerToString(opt.color)),(double)(opt.color>>16)/255.0, (double)((opt.color>>8)&0xFF)/255.0, (double)(opt.color&0xFF)/255.0);
	SetPenColor(color_name);
	
}
static inline void clearStack() //�����Ԫ��ջ����Ч�ڻ��� 
{
	if (!stack_top) return;
	rend_x = stack_w;
	while (stack_top) {
		rend_x-=line_obj_stack[stack_top]->w;
		line_obj_stack[stack_top]->x = rend_x;
		line_obj_stack[stack_top]->y = rend_y;
		line_obj_stack[stack_top]->align_h=stack_align_h;
		stack_top--;
	}
	rend_y -= stack_h;
	stack_w=stack_h=stack_align_h=0;
}
static inline void addStack(RENDER *node)//���ڵ�����Ԫ��ջ 
{
	if (stack_w + node->w > area_w) 
		clearStack();
	stack_w += node->w;
	stack_h = max(stack_h, node->h);
	stack_align_h = max(stack_align_h, node->align_h);
	line_obj_stack[++stack_top] = node;
}

RENDER* buildRenderTree(DOM *root,PRINT_OPT opt)
{
	if (opt.states&SMASHED||root->literal==NULL) return NULL;
	RENDER *now = addRenderNode(0.0,0.0,0.0,opt,root->literal,RN(root->tagtype),RN(getAttr(root,"id")));
	opt=updateOpt(root,opt);
	SAVEPEN(now);
	applyPrintOpt(opt);
	if (opt.states&SMASHED) return NULL;
	now->h=0;
	now->w=0;
	now->x=0;
	now->y=0;
	now->align_h=0;
	
	/* Before-effects */
	if (StringEqual(root->tagtype, "h1")) //������Ҫ���� 
		clearStack();
	else if (StringEqual(root->tagtype, "h2")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h3")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h4")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h5")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h6")) 
		clearStack();
	else if (StringEqual(root->tagtype, "p")) 
		clearStack();
	
	else if (StringEqual(root->tagtype, "text")) { //���� 
		now->h=GetFontHeight();
		now->align_h=GetFontHeight()-GetFontDescent(); //��ȡ�����baseline 
		now->w=TextStringWidth(RN(root->literal));
	}
	else if (StringEqual(root->tagtype, "title")) { //��ȡ��ҳ���� 
		opt.states|=SMASHED;
		title = getSubtext(root);
		
	}
	else if (StringEqual(root->tagtype, "style")) { //��ȡCSS���ݲ�Ӧ�� 
		opt.states|=SMASHED;
		globalCSS = getSubtext(root);
		
	}
	else if (StringEqual(root->tagtype, "a")) { //�����Ӵ��� 
		now->h=GetFontHeight();
		now->align_h=GetFontHeight()-GetFontDescent();
		string strtmp;
		if (strtmp=getAttr(root,"href")) {
			opt.states|=HYPER;
			opt.color=0x0000FF;
			hrefs[tot_href++]=RN(strtmp);
		}
	}
	else if (StringEqual(root->tagtype, "br")) { //���� 
		clearStack();
	}
	else if (StringEqual(root->tagtype, "hr")) { // ˮƽ�ָ� 
		clearStack();
		opt.states|=HR;
		now->h=GetFontHeight();
		now->align_h=GetFontHeight();
		now->w=GetWindowWidth();//for further implementation in future
		clearStack();
	}
	
	now->opt=opt;
	
	if (!StringEqual(now->tagtype, "a")) {
		for (size_t i=0;i<root->child_count&&!(opt.states&SMASHED);i++) {
			RENDER *tmp = buildRenderTree(root->childs[i],opt);
			if (tmp) {
				now->childs[now->child_count]=tmp;
				now->child_count++;
				now->h=max(now->h, tmp->h);
				now->w+=tmp->w;
			}
			
		}
	} else {
		now->print_text=getSubtext(root);
		now->h=GetFontHeight()*0.8;
		now->w=TextStringWidth(now->print_text)*1.2;
		now->child_count=0;
	}
	if (now->child_count==0)
		addStack(now);
	/* After effects */
	if (StringEqual(root->tagtype, "h1")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h2")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h3")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h4")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h5")) 
		clearStack();
	else if (StringEqual(root->tagtype, "h6")) 
		clearStack();
	else if (StringEqual(root->tagtype, "p")) 
		clearStack();
	
	RECOVERPEN(now);
	return now;
}
void mainRender(float_64 x,float_64 y,float_64 w,float_64 h,RENDER *root) //����Ⱦ������DFS��ʽ����Render Tree 
{
	SAVEPEN(main);
	applyPrintOpt(root->opt);
	float_64 base_x=root->x+area_x+shift_x, base_y=root->y-root->align_h+area_y+shift_y;
	if (root->child_count==0) {
		MovePen(base_x,base_y);
		bool enable_output = !(base_x-area_x>area_w||area_y-base_y>area_h+root->h||base_y>area_y||base_x+root->w<area_x);// ������Ⱦ��Χ��ʹ����� 

		MovePen(base_x,base_y);
		if (enable_output && root->opt.states&UNDERLINE) {
			SAVEPEN(und);
			SetPenSize(GetPointSize()/9);
			DrawLine(root->w,0),DrawLine(-root->w,0);
			RECOVERPEN(und);
		}
		if (enable_output && root->opt.states&DELETED) {
			SAVEPEN(del);
			SetPenSize(GetPointSize()/9);
			MovePen(root->x+area_x+shift_x,root->y-root->align_h+GetFontAscent()*0.3+area_y+shift_y);
			DrawLine(root->w,0),DrawLine(-root->w,0);
			MovePen(base_x,base_y);
			RECOVERPEN(del);
		}
		if (enable_output && root->opt.states&HR) {
			SAVEPEN(del);
			SetPenSize(2);
			MovePen(root->x+area_x+shift_x,root->y-root->h*0.6+area_y+shift_y);
			DrawLine(GetWindowWidth(),0),DrawLine(-GetWindowWidth(),0);
			MovePen(root->x,root->y-root->h*0.4);
			DrawLine(GetWindowWidth(),0),DrawLine(-GetWindowWidth(),0);
			MovePen(base_x,base_y);
			RECOVERPEN(del);
		}

		if (!(root->opt.states&HYPER)) {
			size_t max_chars = area_w/GetFontWidth(); //Overflow relieving 
			enable_output ? DrawTextString(SubString(root->print_text,0,max_chars)) : 0;
		} else {
			if (enable_output && button(GenUIID(now_button),base_x,base_y,root->w,root->h,root->print_text)) {
				redirect=hrefs[now_button];
				puts(redirect);
				return;
			}
			now_button++;
			return;
	    }
	}
	
	for (size_t i=0;i<root->child_count;i++) {
		RENDER *child = root->childs[i];
		mainRender(x,y,w,h,child); 
	}
	RECOVERPEN(main);
}

void drawHtml(float_64 x,float_64 y,float_64 w,float_64 h)//����HTML����Ҫ���� 
{
	now_button=0;
	area_w=w,area_h=h,area_x=x,area_y=y;
	rend_x=rend_y=0;
	redirect=NULL;
	if (!lazying) { //��Ҫˢ�£������ػ� 
		dom=htmlParser(html_str);
		tot_href=0;
		render_node_count=0;
	 	rtree=buildRenderTree(dom,INIT_OPT);
	 	clearStack();
	}
	mainRender(x,y,w,h,rtree);
	if (wait_for_jump && StringLength(wait_for_jump)>1)//��ҳ��ת 
		jumpByID(wait_for_jump),wait_for_jump=NULL;
	lazying=TRUE; 
}

