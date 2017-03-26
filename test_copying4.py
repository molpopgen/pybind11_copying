import copying4

o=copying4.Object(4)

def work_on_subobj(s):
    print(s.x)

def work_on_obj(o):
    print(o.s.x)

po = copying4.ProcessSubobject()

copying4.doit(o,po,work_on_obj)
