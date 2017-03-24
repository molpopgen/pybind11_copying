import copying

p = copying.make_pop()

def fitness(dip,g,e):
    """
    Each call to this function
    results in g,e being copied,
    which gets expensive
    """
    sum=0.
    for gi in g[dip[0]].mutations:
        sum+=e[gi].s
    for gi in g[dip[1]].mutations:
        sum+=e[gi].s
    return sum

class Recorder:
    """
    Calls to this object
    work w/o any extra copies
    """
    def __init__(self):
        self.data=[]
    def __call__(self,pop):
        return
        for i in pop.ind:
            self.data.append(fitness(i,pop.gametes,pop.effects))

record=Recorder()
copying.process_pop(p,fitness,record)
print(record.data)
