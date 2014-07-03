
#!/usr/bin/python
# -*- coding:utf-8; tab-width:4; mode:python -*-
import random
import os
import thread
from time import *

import heapq
import sys,csv
from numpy import *

idp=0

def Ejecutar(Estado):

	idp = (Estado.get_id())
	posJ = (Estado.get_posx(), Estado.get_posy())
	ratio = (Estado.get_ratiox(), Estado.get_ratioy())
	grid = (Estado.get_gridx(), Estado.get_gridy())
	posE = (Estado.get_posxE(), Estado.get_posyE())
	tam = (Estado.get_tamx(),Estado.get_tamy())
	mapa_aux = Estado.get_mapa()
	f = open ("fichero.txt", "a")
    f.write("idp\n")
    f.write(str(idp))
    f.close()
	mapa=[]
	cont = 0
	for x in range (0,grid[0]):
		l=[]
		for y in range (0,grid[1]):
			l.append(mapa_aux[cont])
			cont = cont + 1
		mapa.append(l)
	l=[]
	m=Map(celda=tam,tablero=grid)
	for x in range(0,m.tablero[0]):
		for y in range(0,m.tablero[1]):
            m.ModificaMapaS(int(mapa[x][y]),(x,y))
	Ggrid=GrafoMapa(m)
	P=ProblemaGrafo(idp,(ratio[0],ratio[1]),Ggrid,(posJ[0],posJ[1]),(posE[0],posE[1]),(grid[0],grid[1]),(tam[0],tam[1]))
	n=BuscaSolucionesA(P)
	f = open ("fichero.txt", "a")
    f.write("Buscar soluciones")
    f.close()
    l=n.camino()
	for a in l:
		m.ModificaMapaS(6,(a.estado[1],a.estado[0]))
	f = open ("fichero.txt", "a")
    f.write("HHHOLA")
    f.close()
	for a in l:
		if a.accion<>None:
			Estado.add_mov(a.accion['Acc'])
			f = open ("fichero.txt", "a")
            f.write(str(a.accion['Acc']))
            f.close()



class Grafo:
	def __init__(self,tipo='Dir',nombre='Noname'):
		self.g={}
		self.tipo=tipo
		self.id=idp
		self.NumNodos=0
		self.NumArc=0
	def addNodo(self,Nodo):
		if not Nodo in self.g.keys():
			self.g[Nodo]=[]
	def addArco(self,NodoOrigen,NodoDestino,InfArco):
		self.addNodo(NodoOrigen)
		self.addNodo(NodoDestino)
		self.g[NodoOrigen].append((InfArco,NodoDestino))
		if not (self.tipo=='Dir'):
			self.g[NodoDestino].append((InfArco,NodoOrigen))
	def nodosSucesores(self,Nodo,Ratio):
		return self.g[Nodo]
	def heuristica(self,estado1,estado2):
		pass

class ProblemaGrafo():
	def __init__(self,idp,Ratio,Grafo,NodoInicial,NodoFinal,Grid,Tam):
		self.id=idp
		self.ratio=Ratio
		self.estInit=(NodoInicial[0],NodoInicial[1])
		self.estadoFin=(NodoFinal)
		self.graf=Grafo
		self.tam=Tam
		self.grid=Grid
	def estadoValido(self,Nodo):
		return Nodo in self.graf.g.keys()
	def sucesor(self,Nodo,Ratio,Tam):
		return self.graf.nodosSucesores(Nodo,Ratio,Tam)
	def meta(self,Nodo):
        return Nodo==self.estadoFin
	def estadoInit(self):
		return self.estInit
	def heuristica(self,estado):
		return self.graf.heuristica(estado,self.estadoFin)


class GrafoMapa(Grafo):
	def __init__(self,map,h='EUCLIDEA'):
		self.dato=map
		self.id=idp
		self.h=h
	def addNodo(self,Nodo):
		pass
	def addArco(self,NodoOrigen,NodoDestinod,InfArco):
		pass
	def nodosSucesores(self,Nodo,Ratio,Tam):
		f,c=Nodo
		valido=[True,True,True,True,True,True,True,True]
		anchura,altura=self.dato.tablero
		if (c%Tam[0]<0.5):
		    c = c/Tam[0]
			if (f%Tam[1]<0.5):
			    f = f/Tam[1]
#                        falta comprobar altura y anchura
				if (Ratio[0]%2==0):
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2)-1,f+(Ratio[1]/2)-2,f+(Ratio[1]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)-2,c+(Ratio[0]/2))
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2)-1,c+(Ratio[0]/2)-2,c+(Ratio[0]/2))
				else:
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2)-1,f+(Ratio[1]/2)-2,f+(Ratio[1]/2),c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
			else:
			    f = f/Tam[1]
				if (Ratio[0]%2==0):
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2)+1,f-(Ratio[1]/2),f-(Ratio[1]/2)+2,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2)-1,c+(Ratio[0]/2)-2,c+(Ratio[0]/2))
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2)-1,c+(Ratio[0]/2)-2,c+(Ratio[0]/2))
				else:
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2)+1,f-(Ratio[1]/2),f-(Ratio[1]/2)+2,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
		else:
		    c = c/Tam[0]
			if (f%Tam[1]<0.5):
			    f = f/Tam[1]
				if (Ratio[0]%2==0):
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)+2)
                        fin=(f+(Ratio[1]/2)-1,f+(Ratio[1]/2)-2,f+(Ratio[1]/2),c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)+2)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
				else:
				    if (Ratio[1]%2==0):
				        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2)-1,f+(Ratio[1]/2)-2,f+(Ratio[1]/2),c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
			else:
			    f = f/Tam[1]
				if (Ratio[0]%2==0):
				    if (Ratio[1]%2==0):
                        inicio=(f-(Ratio[1]/2)+1,f-(Ratio[1]/2),f-(Ratio[1]/2)+2,c-(Ratio[0]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)+2)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)+2)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
				else:
				    if (Ratio[1]%2==0):
				        inicio=(f-(Ratio[1]/2)+1,f-(Ratio[1]/2),f-(Ratio[1]/2)+2,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
                    else:
                        inicio=(f-(Ratio[1]/2),f-(Ratio[1]/2)-1,f-(Ratio[1]/2)+1,c-(Ratio[0]/2),c-(Ratio[0]/2)-1,c-(Ratio[0]/2)+1)
                        fin=(f+(Ratio[1]/2),f+(Ratio[1]/2)-1,f+(Ratio[1]/2)+1,c+(Ratio[0]/2),c+(Ratio[0]/2)-1,c+(Ratio[0]/2)+1)
		fi = open ("fichero.txt", "a")
        fi.write("Despues del tocho1")
        fi.close()
		for y in range(inicio[0],fin[0]):
		    if ((fin[5]<0 | fin[5]>anchura | y<0 | y>altura) | (self.dato.mapa[fin[5]][y]!=-1)):
                valido[0]=False
            if ((inicio[4]<0 | inicio[4]>anchura | y<0 | y>altura) | (self.dato.mapa[inicio[4]][y]!=-1)):
                valido[4]=False
		for y in range(inicio[1],fin[1]):
		    if ((fin[5]<0 | fin[5]>anchura | y<0 | y>altura) | (self.dato.mapa[fin[5]][y]!=-1)):
                valido[7]=False
            if ((inicio[4]<0 | inicio[4]>anchura | y<0 | y>altura) | (self.dato.mapa[inicio[4]][y]!=-1)):
                valido[5]=False
		for y in range(inicio[2],fin[2]):
		    if ((fin[5]<0 | fin[5]>anchura | y<0 | y>altura) | (self.dato.mapa[fin[5]][y]!=-1)):
                valido[1]=False
            if ((inicio[4]<0 | inicio[4]>anchura | y<0 | y>altura) | (self.dato.mapa[inicio[4]][y]!=-1)):
                valido[3]=False

		for x in range(inicio[3],fin[3]):
		    if ((x<0 | x>anchura | fin[2]<0 | fin[2]>altura) | (self.dato.mapa[x][fin[2]]!=-1)):
                valido[2]=False
            if ((x<0 | x>anchura | inicio[1]<0 | inicio[1]>altura) | (self.dato.mapa[x][inicio[1]]!=-1)):
                valido[6]=False
		for x in range(inicio[4],fin[4]):
		    if ((x<0 | x>anchura | fin[2]<0 | fin[2]>altura) | (self.dato.mapa[x][fin[2]]!=-1)):
                valido[3]=False
            if ((x<0 | x>anchura | inicio[1]<0 | inicio[1]>altura) | (self.dato.mapa[x][inicio[1]]!=-1)):
                valido[5]=False
		for x in range(inicio[5],fin[5]):
		    if ((x<0 & x>anchura | fin[2]<0 | fin[2]>altura) | (self.dato.mapa[x][fin[2]]!=-1)):
                valido[1]=False
            if ((x<0 | x>anchura | inicio[1]<0 | inicio[1]>altura) | (self.dato.mapa[x][inicio[1]]!=-1)):
                valido[7]=False
        mv=[]
		if (valido[0]):
			mv.append(({'Acc':'R','val':3},(f,c+1)))
		if (valido[1]):
            mv.append(({'Acc':'I','val':3},(f+1,c+1)))
		if (valido[2]):
			mv.append(({'Acc':'D','val':3},(f+1,c)))
		if (valido[3]):
			mv.append(({'Acc':'W','val':3},(f+1,c-1)))
		if (valido[4]):
			mv.append(({'Acc':'L','val':3},(f,c-1)))
		if (valido[5]):
			mv.append(({'Acc':'O','val':3},(f-1,c-1)))
		if (valido[6]):
			mv.append(({'Acc':'U','val':3},(f-1,c)))
		if (valido[7]):
			mv.append(({'Acc':'E','val':3},(f-1,c+1)))
		return mv
	def heuristica(self,nodo1,nodo2):
		y1,x1=nodo1
		y2,x2=nodo2
		s=0.0
		if self.h=='EUCLIDEA':
			s= ((x1-x2)**2.0+(y1-y2)**2.0)**(1.0/2.0)
		elif self.h=='CUNIFORME':
			s=0.0
		else:
			s= abs(x2-x1)+abs(y2-y1)
		return s
class AlmacenNodos:
	def __init__(self,nombre="NoName"):
		self.id=idp
	def add(self,elem):
		pass
	def esVacia(self):
		pass
	def get(self):
		pass
	def esta(self,id):
		pass

class AlmacenOrdenado(AlmacenNodos):
	def __init__(self):
		AlmacenNodos.__init__(self,'ListOrdenada')
		self.visit={}
		self.heap=[]
	def add(self,elem):
		val,rep,dat=elem
		if rep in self.visit.keys() and self.visit[rep]<=val:
			return
		else:
			self.visit[rep]=val
			heapq.heappush(self.heap,(val,dat))
	def esVacia(self):
		return len(self.heap)==0
	def get(self):
		val,dat=heapq.heappop(self.heap)
		return dat
	def esta(self,rep):
		return rep in self.visit.keys()

class NodoArbol:
	id=0
	def __init__(self,padre,acc,estado,inf=None,val=0):
		self.padre=padre
		self.accion=acc
		self.estado=estado
		self.inf=inf
		self.id=NodoArbol.id
		self.val=val
		NodoArbol.id=NodoArbol.id+1

	def camino(self):
		p=[]
		n=self
		while n<>None:
			p.insert(0,n)
			n=n.padre
		return p


def GeneraNodo(p,n,ac,est,estr='A'):
	MUCHO=10000000
	prof=n.inf['Prof']+1
	coste=n.inf['Coste']+ac['val']
	h=p.heuristica(est)
	if estr=='A':
		val=coste+h
	elif estr=='PROF':
		val=MUCHO-prof
	elif estr=='ANCH':
		val=prof
	elif estr=='COST':
		val=coste
	else:
		val=h
	return NodoArbol(n,ac,est,{'Prof':prof,'Coste':coste,'h':h},val)

def BuscaSolucionesA(prob,maxIter=-1,estr='A'):
	frontera=AlmacenOrdenado()
	h=prob.heuristica(prob.estadoInit())
	n=NodoArbol(None,None,prob.estadoInit(),{'Prof':0,'Coste':0,'h':h},0)
	solucion=None
	frontera.add((n.val,n.estado,n))
	while (not frontera.esVacia()) and (solucion==None):
	    f = open ("fichero.txt", "a")
        f.write("Dentro\n")
        f.close()
		n=frontera.get()
		f = open ("fichero.txt", "a")
        f.write(str(prob.estadoInit()))
        f.write(str(prob.ratio))
        f.close()
		if prob.meta(n.estado):
			solucion=n
		else:
		    f = open ("fichero.txt", "a")
            f.write("No meta\n")
            f.close()
			for ac,est in prob.sucesor(n.estado,prob.ratio,prob.tam):
			    f = open ("fichero.txt", "a")
                f.write("For\n")
                f.write(str(ac))
                f.write(str(est))
                f.close()
				if not frontera.esta(est):
				    f = open ("fichero.txt", "a")
                    f.write("If\n")
                    f.close()
					h=GeneraNodo(prob,n,ac,est,estr)
					f = open ("fichero.txt", "a")
                    f.write("GeneraNodo\n")
                    f.close()
					frontera.add((h.val,h.estado,h))
					f = open ("fichero.txt", "a")
                    f.write("add\n")
                    f.close()
	return solucion

class Map:
	def __init__(self,celda=(20,20),tablero=(50,50),m=None):
		self.celda=celda
		self.tablero=tablero
		self.size = width,height = tablero[0]*celda[0],tablero[1]*celda[1]
		if m==None:
			self.CreaMap(self.tablero[0],self.tablero[1])
		else:
			self.mapa=m

	def CreaMap(self,w,h):
		self.mapa=[]
		for y in range(h):
			self.mapa.append([])
			for x in range(w):
				self.mapa[y].append(0)

	def ModificaMapaS(self,celdaIndx,pos):
		self.mapa[pos[1]][pos[0]]=celdaIndx

	def Busca(self,id):
		l=[]
		for fila in range(len(self.mapa)):
			for colum in range(len(self.mapa[fila])):
				if self.mapa[fila][colum]==id:
					l.append((fila,colum))
		return l
