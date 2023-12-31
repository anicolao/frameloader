/**
  ******************************************************************************
  * @file    scripts.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   This file describes the sending of JavaScript codes to a client
  *
  ******************************************************************************
  */ 

void sendJS_A(WiFiClient client)
{
client.println(
"var srcBox,srcImg,dstImg;\r\n"
"var epdArr,epdInd,palArr;\r\n"
"var curPal;\r\n"
"function getElm(n){return document.getElementById(n);}\r\n"
"function setInn(n,i){ document.getElementById(n).innerHTML=i;}\r\n"

"function processFiles(files){\r\n"
    "var file=files[0];\r\n"
    "var reader=new FileReader();\r\n"
    "srcImg=new Image();\r\n"
    "reader.onload=function(e){\r\n"
        "setInn('srcBox','<img id=\"imgView\" class=\"sourceImage\">');\r\n"
        "var img=getElm('imgView');\r\n"
        "img.src=e.target.result;\r\n"
        "srcImg.src=e.target.result;\r\n"
    "};\r\n"
    
    "reader.readAsDataURL(file);\r\n"
"}\r\n"
//-------------------------------------------
"function drop(e){\r\n"
    "e.stopPropagation();\r\n"
    "e.preventDefault();\r\n"
    "var files=e.dataTransfer.files;\r\n"
    "processFiles(files);\r\n"
"}\r\n"
//-------------------------------------------
"function ignoreDrag(e){\r\n"
    "e.stopPropagation();\r\n"
    "e.preventDefault();\r\n"
"}\r\n"
//-------------------------------------------
"function getNud(nm,vl){\r\n"
    "return '<td class=\"comment\">'+nm+':</td>'+\r\n"
    "'<td><input id=\"nud_'+nm+'\" class=\"nud\"type=\"number\" value=\"'+vl+'\"/></td>';\r\n"
"}\r\n"
//-------------------------------------------
"function Btn(nm,tx,fn){\r\n"
    "return '<div><label class=\"menu_button\" for=\"_'+nm+'\">'+tx+'</label>'+\r\n"
    "'<input class=\"hidden_input\" id=\"_'+nm+'\" type=\"'+\r\n"
    "(nm==0?'file\" onchange=\"':'button\" onclick=\"')+fn+'\"/></div>';\r\n"
"}\r\n"
//-------------------------------------------
"function RB(vl,tx){\r\n"
    "return ((vl%3)>0?' ':'<br>') + '<input type=\"radio\" name=\"kind\" value=\"m'+vl+\r\n"
	"'\" onclick=\"rbClick('+vl+');\"'+(vl==0?'checked=\"true\"':'')+'/>'+tx;\r\n"
"}\r\n"
//-------------------------------------------
"window.onload = function(){\r\n"
    "srcBox = getElm('srcBox');\r\n"
    "srcBox.ondragenter=ignoreDrag;\r\n"
    "srcBox.ondragover=ignoreDrag;\r\n"
    "srcBox.ondrop=drop;\r\n"
    "srcImg=0;\r\n"
    "epdInd=0;\r\n"
  
"palArr=[[[0,0,0],[255,255,255]],\r\n"
"[[0,0,0],[255,255,255],[255,0,0]],\r\n"
"[[0,0,0],[255,255,255],[127,127,127]],\r\n"
"[[0,0,0],[255,255,255],[127,127,127],[255,0,0]],\r\n"
"[[0,0,0],[255,255,255]],\r\n"
"[[0,0,0],[255,255,255],[220,180,0]]];\r\n"
  
"epdArr=[[200,200,0],[200,200,3],[152,152,5],\r\n"
"[122,250,0],[104,212,1],[104,212,5],\r\n"
"[176,264,0],[176,264,1],[176,264,5],\r\n"
"[128,296,0],[128,296,1],[128,296,5],\r\n"
"[400,300,0],[400,300,1],[400,300,5],\r\n"
"[640,384,0],[640,384,1],[640,384,5]];\r\n"
  
"setInn('BT',\r\n"
"Btn(0,'Select image file','processFiles(this.files);')+\r\n"
"Btn(1,'Level: mono','procImg(true,false);')+\r\n"
"Btn(2,'Level: color','procImg(true,true);')+\r\n"
"Btn(3,'Dithering: mono','procImg(false,false);')+\r\n"
"Btn(4,'Dithering: color','procImg(false,true);')+\r\n"
"Btn(5,'Upload image','uploadImage();'));\r\n"

"setInn('XY',getNud('x','0')+getNud('y','0'));\r\n"
"setInn('WH',getNud('w','200')+getNud('h','200'));\r\n"
  
"setInn('RB',RB(0,'1.54')+RB(1,'1.54b')+RB(2,'1.54c')+\r\n"
"RB(3,'2.13')+RB(4,'2.13b')+RB(5,'2.13c')+\r\n"
"RB(6,'2.7&ensp;')+RB(7,'2.7b&ensp;')+\r\n"
"RB(9,'2.9&ensp;')+RB(10,'2.9b&ensp;')+RB(11,'2.9c&ensp;')+\r\n"
"RB(12,'4.2&ensp;')+RB(13,'4.2b&ensp;')+RB(14,'4.2c&ensp;')+\r\n"
"RB(15,'7.5&ensp;')+RB(16,'7.5b&ensp;')+RB(17,'7.5c&ensp;'));\r\n"
"}\r\n"
//-------------------------------------------
"function rbClick(index){\r\n"
    "getElm('nud_w').value=""+epdArr[index][0];\r\n"
    "getElm('nud_h').value=""+epdArr[index][1];\r\n"
    "epdInd=index;\r\n"
"}\r\n");
}

void sendJS_B(WiFiClient client)
{
client.println(
"var source;\r\n"
"var dX, dY, dW, dH, sW, sH;\r\n"
//-------------------------------------------
"function getVal(p, i){\r\n"
    "if((p.data[i]==0x00) && (p.data[i+1]==0x00))return 0;\r\n"
    "if((p.data[i]==0xFF) && (p.data[i+1]==0xFF))return 1;\r\n"
    "if((p.data[i]==0x7F) && (p.data[i+1]==0x7F))return 2;\r\n"
    "return 3;\r\n"
"}\r\n"
//-------------------------------------------
"function setVal(p,i,c){\r\n"
    "p.data[i]=curPal[c][0];\r\n"
    "p.data[i+1]=curPal[c][1];\r\n"
    "p.data[i+2]=curPal[c][2];\r\n"
    "p.data[i+3]=255;\r\n"
"}\r\n"
//-------------------------------------------
"function addVal(c,r,g,b,k){\r\n"
    "return[c[0]+(r*k)/16,c[1]+(g*k)/16,c[2]+(b*k)/16];\r\n"
"}\r\n"
//-------------------------------------------
"function getErr(r,g,b,stdCol){\r\n"
    "r-=stdCol[0];\r\n"
    "g-=stdCol[1];\r\n"
    "b-=stdCol[2];\r\n"
    "return r*r + g*g + b*b;\r\n"
"}\r\n"
//-------------------------------------------
"function getNear(r,g,b){\r\n"
    "var ind=0;\r\n"
    "var err=getErr(r,g,b,curPal[0]);\r\n"
    "for (var i=1;i<curPal.length;i++)\r\n"
    "{\r\n"
        "var cur=getErr(r,g,b,curPal[i]);\r\n"
        "if (cur<err){err=cur;ind=i;}\r\n"
    "}\r\n"
    "return ind;\r\n"
"}\r\n");
}

void sendJS_C(WiFiClient client)
{
    client.println(
"function procImg(isLvl,isRed){\r\n"
    "if (document.getElementsByClassName('sourceImage').length == 0){\r\n"
        "alert('First select image');\r\n"
        "return;\r\n"
    "}\r\n"
    
    "var palInd=epdArr[epdInd][2];\r\n"
    
    "if (isRed&&((palInd&1)==0)){\r\n"
        "alert('This white-black display');\r\n"
        "return;\r\n"
    "}\r\n"
    
    "if (!isRed)palInd=palInd&0xFE;\r\n"
    "curPal=palArr[palInd];\r\n"
    "getElm('dstBox').innerHTML=\r\n"
    "'<span class=\"title\">Processed image</span><br><canvas id=\"canvas\"></canvas>';\r\n" 
    "var canvas=getElm('canvas');\r\n"
    "sW=srcImg.width;\r\n"
    "sH=srcImg.height;\r\n"
    "source=getElm('source');\r\n"
    "source.width=sW;\r\n"
    "source.height=sH;\r\n"
    "source.getContext('2d').drawImage(srcImg,0,0,sW,sH);\r\n"
    "dX=parseInt(getElm('nud_x').value);\r\n"
    "dY=parseInt(getElm('nud_y').value);\r\n"
    "dW=parseInt(getElm('nud_w').value);\r\n"
    "dH=parseInt(getElm('nud_h').value);\r\n"
    
    "if((dW<3)||(dH<3)){\r\n"
        "alert('Image is too small');\r\n"
        "return;\r\n"
    "}\r\n"
    
    "canvas.width=dW;\r\n"
    "canvas.height=dH;\r\n"
    "var index=0;\r\n"
    "var pSrc=source.getContext('2d').getImageData(0,0,sW,sH);\r\n"
    "var pDst=canvas.getContext('2d').getImageData(0,0,dW,dH);\r\n"
    
    "if(isLvl){\r\n"
        "for (var j=0;j<dH;j++){\r\n"
            "var y=dY+j;\r\n"
            "if ((y<0)||(y>=sH)){\r\n"
                "for (var i=0;i<dW;i++,index+=4) setVal(pDst,index,(i+j)%2==0?1:0);\r\n"
                "continue;\r\n"
            "}\r\n"
            
            "for (var i=0;i<dW;i++){\r\n"
                "var x=dX+i;\r\n"
                
                "if ((x<0)||(x>=sW)){\r\n"
                    "setVal(pDst,index,(i+j)%2==0?1:0);\r\n"
                    "index+=4;\r\n"
                     "continue;\r\n"
                 "}\r\n"
                 
                 "var pos=(y*sW+x)*4;\r\n"     
                 "setVal(pDst,index,getNear(pSrc.data[pos],pSrc.data[pos+1],pSrc.data[pos+2]));\r\n"
                 "index+=4;\r\n"
             "}\r\n"
         "}\r\n"
    "}else{\r\n"
        "var aInd=0;\r\n"
        "var bInd=1;\r\n"
        "var errArr=new Array(2);\r\n"
        "errArr[0]=new Array(dW);\r\n"
        "errArr[1]=new Array(dW);\r\n"
        
        "for (var i=0;i<dW;i++)\r\n"
            "errArr[bInd][i]=[0,0,0];\r\n"
            
        "for (var j=0;j<dH;j++){\r\n"
            "var y=dY+j;\r\n"
            
            "if ((y<0)||(y>=sH)){\r\n"
                "for (var i=0;i<dW;i++,index+=4)setVal(pDst,index,(i+j)%2==0?1:0);\r\n"  
                "continue;\r\n"
            "}\r\n"
            
            "aInd=((bInd=aInd)+1)&1;\r\n"
            "for (var i=0;i<dW;i++)errArr[bInd][i]=[0,0,0];\r\n"
            
            "for (var i=0;i<dW;i++){\r\n"
                "var x=dX+i;\r\n"
                
                "if ((x<0)||(x>=sW)){\r\n"
                    "setVal(pDst,index,(i+j)%2==0?1:0);\r\n"
                    "index+=4;\r\n"
                    "continue;\r\n"
                "}\r\n"
                
                "var pos=(y*sW+x)*4;\r\n"
                "var old=errArr[aInd][i];\r\n"
                "var r=pSrc.data[pos  ]+old[0];\r\n"
                "var g=pSrc.data[pos+1]+old[1];\r\n"
                "var b=pSrc.data[pos+2]+old[2];\r\n"
                "var colVal = curPal[getNear(r,g,b)];\r\n"
                "pDst.data[index++]=colVal[0];\r\n"
                "pDst.data[index++]=colVal[1];\r\n"
                "pDst.data[index++]=colVal[2];\r\n"
                "pDst.data[index++]=255;\r\n"
                "r=(r-colVal[0]);\r\n"
                "g=(g-colVal[1]);\r\n"
                "b=(b-colVal[2]);\r\n"
                
                "if (i==0){\r\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,7.0);\r\n"
                    "errArr[bInd][i+1]=addVal(errArr[bInd][i+1],r,g,b,2.0);\r\n"
                    "errArr[aInd][i+1]=addVal(errArr[aInd][i+1],r,g,b,7.0);\r\n"
                "}else if (i==dW-1){\r\n"
                    "errArr[bInd][i-1]=addVal(errArr[bInd][i-1],r,g,b,7.0);\r\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,9.0);\r\n"
                "}else{\r\n"
                    "errArr[bInd][i-1]=addVal(errArr[bInd][i-1],r,g,b,3.0);\r\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,5.0);\r\n"
                    "errArr[bInd][i+1]=addVal(errArr[bInd][i+1],r,g,b,1.0);\r\n"
                    "errArr[aInd][i+1]=addVal(errArr[aInd][i+1],r,g,b,7.0);\r\n"
                "}\r\n"
            "}\r\n"
        "}\r\n"
    "}\r\n"
    
    "canvas.getContext('2d').putImageData(pDst,0,0);\r\n"
"}\r\n");
}

void sendJS_D(WiFiClient client)
{
client.println(
"var pxInd,stInd;\r\n"
"var dispW,dispH;\r\n"
"var xhReq,dispX;\r\n"
"var rqPrf,rqMsg;\r\n"

"var prvPx,prvSt;\r\n"
"function ldPrv(){if(xhReq.status!=200){pxInd=prvPx;stInd=prvSt;}}\r\n"
"function svPrv(){prvPx=pxInd;prvSt=stInd;}\r\n"

"function byteToStr(v){return String.fromCharCode((v & 0xF) + 97, ((v >> 4) & 0xF) + 97);}\r\n"
"function wordToStr(v){return byteToStr(v&0xFF) + byteToStr((v>>8)&0xFF);}\r\n"
//-------------------------------------------
"function u_send(cmd,next){\r\n"
    "xhReq.open('POST',rqPrf+cmd, true);\r\n"
    "xhReq.send('');\r\n"
    "if(next)stInd++;\r\n"
    "return 0;\r\n" 
"}\r\n"
//-------------------------------------------
"function u_next(){\r\n"
    "lnInd=0;\r\n"
    "pxInd=0;\r\n"
    "u_send('NEXT_',true);\r\n"
"}\r\n"
//-------------------------------------------
"function u_done(){\r\n"
    "setInn('logTag','Complete!');\r\n"
    "return u_send('SHOW_',true);\r\n"
"}\r\n"
//-------------------------------------------
"function u_loadA(a,k1,k2){\r\n"
    "var x=''+(k1+k2*pxInd/a.length);\r\n"
    "if(x.length>5)x=x.substring(0,5);\r\n"
    "setInn('logTag','Progress: '+x+'%');\r\n"
    "return u_send(rqMsg+wordToStr(rqMsg.length)+'LOAD_',pxInd>=a.length);\r\n"
"}\r\n"
//-------------------------------------------
"function u_loadB(a,k1,k2){\r\n"
    "var x=''+(k1+k2*pxInd/a.length);\r\n"
    "if(x.length>5)x=x.substring(0,5);\r\n"
    "setInn('logTag','Progress: '+x+'%');\r\n"
    "return u_send(rqMsg+wordToStr(rqMsg.length)+'LOAD_',pxInd>=a.length);\r\n"
"}\r\n"
//-------------------------------------------
"function u_dataA(a,c,k1,k2)\r\n"
"{\r\n"
    "rqMsg='';\r\n"
    "svPrv();\r\n"

    "if(c==-1)\r\n"
    "{\r\n"
        "while((pxInd<a.length)&&(rqMsg.length<1000))\r\n"
        "{\r\n"
            "var v=0;\r\n"

            "for (var i=0;i<16;i+=2)\r\n"
            "{\r\n"
                "if(pxInd<a.length)v|=(a[pxInd]<<i);\r\n"
                "pxInd++;\r\n"
            "}\r\n"

            "rqMsg += wordToStr(v);\r\n"    
        "}\r\n"
    "}else{\r\n"  
        "while((pxInd<a.length)&&(rqMsg.length<1000))\r\n"
        "{\r\n"
           "var v=0;\r\n"

           "for (var i=0;i<8;i++)\r\n"
           "{\r\n"
                "if((pxInd<a.length)&&(a[pxInd]!=c))v|=(128>>i);\r\n"
                "pxInd++;\r\n"
            "}\r\n"

            "rqMsg += byteToStr(v);\r\n"
        "}\r\n"
    "}\r\n"
  
    "return u_loadA(a,k1,k2);\r\n"
"}\r\n"
//-------------------------------------------
"function u_dataB(a,k1,k2){\r\n"
    "var x;\r\n"
    "rqMsg='';\r\n"
    "svPrv();\r\n"

    "while(rqMsg.length<1000)\r\n"
    "{\r\n"
        "x=0;\r\n"

        "while(x<122)\r\n"
        "{\r\n"
            "var v=0;\r\n"
            "for (var i=0;(i<8)&&(x<122);i++,x++)if(a[pxInd++]!=0)v|=(128>>i);\r\n"
            "rqMsg += byteToStr(v);\r\n" 
        "}\r\n"
    "}\r\n"
    
    "return u_loadB(a,k1,k2);\r\n" 
"}\r\n" 
//-------------------------------------------
"function uploadImage(){\r\n"
    "var c=getElm('canvas');\r\n"
    "var w=dispW=c.width;\r\n"
    "var h=dispH=c.height;\r\n"
    "var p=c.getContext('2d').getImageData(0,0,w,h);\r\n"
    "var a=new Array(w*h);\r\n"
    "var i=0;\r\n"
    "for(var y=0;y<h;y++)for(var x=0;x<w;x++,i++)a[i]=getVal(p,i<<2);\r\n"
    "dispX=0;\r\n"
    "pxInd=0;\r\n"
    "stInd=0;\r\n"
    "xhReq=new XMLHttpRequest();\r\n"
    "rqPrf='http://'+getElm('ip_addr').value+'/';\r\n"
    "var init='EPD'+byteToStr(epdInd);\r\n"

    "if (epdInd==3){\r\n"  
        "xhReq.onload=xhReq.onerror = function(){\r\n"
            "ldPrv();\r\n"
            "if(stInd==0)return u_dataB(a,0,100);\r\n"
            "if(stInd==1)return u_done();\r\n"
        "};\r\n"
        
        "return u_send(init,false);\r\n"   
    "}\r\n"

    "if (epdInd>=16){\r\n"  
        "xhReq.onload=xhReq.onerror=function(){\r\n"
            "ldPrv();\r\n"
            "if(stInd==0)return u_dataA(a,-1,0,100);\r\n"
            "if(stInd==1)return u_done();\r\n"
        "};\r\n"
        
        "return u_send(init,false);\r\n"   
    "}\r\n"
    
    "if ((epdInd%3)==0){\r\n"   
        "xhReq.onload=xhReq.onerror=function(){\r\n"
            "ldPrv();\r\n"
            "if(stInd==0)return u_dataA(a,0,0,100);\r\n"
            "if(stInd==1)return u_done();\r\n"
        "};\r\n"
        
        "return u_send(init,false);\r\n"       
    "}else{\r\n"
        "xhReq.onload=xhReq.onerror=function(){\r\n"
            "ldPrv();\r\n"
            "if(stInd==0)return u_dataA(a,(epdInd==1)?-1:0,0,50);\r\n"
            "if(stInd==1)return u_next();\r\n"
            "if(stInd==2)return u_dataA(a,3,50,50);\r\n" 
            "if(stInd==3)return u_done();\r\n"
        " };\r\n"
        
        "return u_send(init,false);\r\n"
    "}\r\n"
"}\r\n");
}
