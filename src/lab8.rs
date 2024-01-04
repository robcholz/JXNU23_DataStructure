mod murphy;

#[derive(Debug)]
struct MGraphData {
    vertex_num: u64,
    edge_num: u64,
    values: std::string::String,
    connections: Vec<Vec<u64>>,
}

// vertex type, edge type
#[derive(Clone)]
#[derive(Debug)]
struct MGraph<VT, ET> {
    vertex_num: u64,
    edge_num: u64,
    vertexes: Vec<VT>,
    edges: Vec<Vec<ET>>,
}

#[derive(Debug)]
#[derive(Clone)]
struct EdgeNode {
    adjacent_vertex: u64,
    next: Option<Box<EdgeNode>>,
}

#[derive(Clone)]
#[derive(Debug)]
struct VertexNode<VT> {
    vertex: VT,
    first_edge: Option<Box<EdgeNode>>,
}

#[derive(Debug)]
struct LinkedGraph<T> {
    vertex_num: u64,
    edge_num: u64,
    adjacent_list: Vec<VertexNode<T>>,
}

impl EdgeNode {
    pub fn new() -> Self {
        return Self {
            adjacent_vertex: 0,
            next: None,
        };
    }

    pub fn set_next(&mut self, next: Option<Box<EdgeNode>>) {
        self.next = next;
    }

    pub fn set_adjacent_vertex(&mut self, vertex: u64) {
        self.adjacent_vertex = vertex;
    }
}

impl<VT> VertexNode<VT> {
    pub fn new(vertex: VT) -> Self {
        Self {
            vertex,
            first_edge: None,
        }
    }

    pub fn set_vertex(&mut self, vertex: VT) {
        self.vertex = vertex;
    }

    pub fn set_edge(&mut self, edge: Option<Box<EdgeNode>>) {
        self.first_edge = edge;
    }
}

impl<T> LinkedGraph<T> {
    pub fn new(data_path: &str, direction: bool) -> LinkedGraph<String> {
        let data: MGraphData = MGraphData {
                    vertex_num: 4,
                    edge_num: 4,
                    values: String::from("01234"),
                    connections: vec![
                        vec![0, 1, 56],
                        vec![0, 2, 34],
                        vec![0, 3, 78],
                        vec![2, 3, 25],
                    ],
                };
        let adjacents = &mut Vec::<VertexNode<String>>::new();
        let vertex_values: Vec<_> = data.values.chars().collect();
        adjacents.reserve(data.vertex_num as usize);
        for i in 0..data.vertex_num {
            // set vertex info
            let mut adj=VertexNode::new(String::from(vertex_values[i as usize]));
            // set edge to null
            adj.set_edge(None);
            adjacents.push(adj);
        } // set up vertex

        for i in 0..data.edge_num {
            let vertex_data = data.connections.get(i as usize);
            let mut edge_node = EdgeNode::new();
            edge_node.set_next(adjacents.get(i as usize).unwrap().first_edge.clone());
            edge_node.set_adjacent_vertex(vertex_data.expect("").get(2).unwrap().clone());
            adjacents.get_mut(i as usize).unwrap().set_edge(Some(Box::from(edge_node)));
            if !direction {
                let mut edge_node = EdgeNode::new();
                let adj= adjacents.get_mut(i as usize).expect("");
                edge_node.set_adjacent_vertex(i);
                edge_node.set_next(adj.first_edge.clone());
                adj.set_edge(Some(Box::from(edge_node)));
            }
        }

        return LinkedGraph {
            vertex_num: data.vertex_num,
            edge_num: data.edge_num,
            adjacent_list: adjacents.to_vec(),
        };
    }
}

impl<VT, ET> MGraph<VT, ET> {
    pub fn new(data_path: &str, direction: bool) -> MGraph<String, u64> {
        let data: MGraphData = MGraphData {
                    vertex_num: 4,
                    edge_num: 4,
                    values: String::from("01234"),
                    connections: vec![
                        vec![0, 1, 56],
                        vec![0, 2, 34],
                        vec![0, 3, 78],
                        vec![2, 3, 25],
                    ],
                };
        let mut vertexes = Vec::<String>::new();
        for char in data.values.chars() {
            vertexes.push(std::string::String::from(char));
        }
        return MGraph {
            vertex_num: data.vertex_num,
            edge_num: data.edge_num,
            vertexes,
            edges: data.connections,
        };
    }
}

fn main(){
    murphy::mur_profiler_module("Lab8");
    let graph = LinkedGraph::<String>::new("", true);
    murphy::mur_profiler_test_lambda("1.TODO", true, &|| {
            println!("{:?}",graph);
            println!();
        });
}