mod murphy;

use std::collections::linked_list::LinkedList;

#[derive(Debug, Clone)]
struct BinaryTreeNode<T> {
    left_node: Box<Option<BinaryTreeNode<T>>>,
    right_node: Box<Option<BinaryTreeNode<T>>>,
    value: T,
}

#[derive(Debug, Clone)]
struct BinaryTree<T> {
    root_node: Option<BinaryTreeNode<T>>,
    size: usize,
}

impl<T> BinaryTreeNode<T> {
    pub fn new(val: T) -> Self {
        BinaryTreeNode {
            left_node: Box::default(),
            right_node: Box::default(),
            value: val,
        }
    }

    pub fn set_value(&mut self, val: T) {
        self.value = val;
    }

    pub fn set_left_node(&mut self, left: Option<BinaryTreeNode<T>>) {
        self.left_node = Box::new(left);
    }

    pub fn set_right_node(&mut self, right: Option<BinaryTreeNode<T>>) {
        self.right_node = Box::new(right);
    }
}

impl<T> BinaryTree<T> {
    pub const fn new() -> Self {
        BinaryTree {
            root_node: None,
            size: 0,
        }
    }

    pub fn set_root_node(&mut self, _root: Option<BinaryTreeNode<T>>) {
        self.root_node = _root;
    }

    // 6. 表达式建树
    pub fn new_from_expr(str_expr: &str) -> BinaryTree<String> {
        let mut symbol_stack: LinkedList<String> = LinkedList::new();
        let mut node_stack: LinkedList<BinaryTreeNode<String>> = LinkedList::new();
        let mut tree: BinaryTree<String> = BinaryTree::new();
        let expr_length: usize = str_expr.len();
        let mut curr: usize = 0;
        let mut size: usize = 0;
        let mut str_at = String::new();
        while curr < expr_length {
            let mut str_index: usize = curr;
            let mut char_at: char = str_expr.chars().nth(str_index).unwrap();
            str_at.clear();
            while char_at.is_alphabetic() {
                str_at.push(char_at);
                str_index += 1;
                size += 1;
                char_at = str_expr.chars().nth(str_index).unwrap();
            }
            // try to get a new node, otherwise str_at is empty
            if !str_at.is_empty() {
                symbol_stack.push_back(str_at.clone());
                node_stack.push_back(BinaryTreeNode::new(str_at.clone()));
            } else {
                if char_at == '(' {
                    symbol_stack.push_back(char_at.to_string());
                } else if char_at == ')' {
                    let mut flag = !symbol_stack.is_empty();
                    let mut temp_root_node: BinaryTreeNode<String>;
                    let mut nodes_num: usize = 0;

                    for symbol in symbol_stack.iter().rev() {
                        flag = !(symbol == "(" || symbol == ",");
                        if flag {
                            nodes_num += 1;
                        } else {
                            break;
                        }
                    } // convert symbols to tree node

                    // this should be the _root value
                    temp_root_node = BinaryTreeNode::new("".to_string());
                    for i in 0..nodes_num {
                        let node = node_stack.pop_back();
                        if i == 0 {
                            temp_root_node.set_right_node(node);
                        } else if i == 1 {
                            temp_root_node.set_left_node(node);
                        }
                    }
                    temp_root_node.set_value(node_stack.pop_back().unwrap().value);
                    node_stack.push_back(temp_root_node); // push back for reservation
                } // reached endpoint of node
            }
            curr += 1;
        }
        tree.set_root_node(node_stack.pop_back());
        tree.size = size;
        return tree;
    }
}

// 1.非递归前序遍历
fn iterate_preorder_nonrecursive(tree: &BinaryTree<String>) -> LinkedList<String> {
    let mut nodes_stack: LinkedList<Box<Option<BinaryTreeNode<String>>>> = LinkedList::new();
    let mut iterated_queue: LinkedList<String> = LinkedList::new();
    nodes_stack.push_back(Box::new(tree.root_node.clone()));
    iterated_queue.push_back(tree.root_node.clone().unwrap().value);
    while !nodes_stack.is_empty() {
        if nodes_stack.back().unwrap().is_some() {
            let node = nodes_stack.back().unwrap().clone().unwrap().left_node;
            nodes_stack.push_back(node.clone());
            if node.is_some() {
                iterated_queue.push_back(node.unwrap().value);
            }
        } else {
            nodes_stack.pop_back();
            if !nodes_stack.is_empty() {
                let node = nodes_stack.pop_back().unwrap().unwrap().right_node;
                nodes_stack.push_back(node.clone());
                if node.is_some() {
                    iterated_queue.push_back(node.unwrap().value);
                }
            }
        }
    }
    return iterated_queue;
}

// 2.层次遍历
fn iterate_levelorder_non_recursive(tree: &BinaryTree<String>) -> LinkedList<String> {
    let mut nodes_stack: LinkedList<Box<Option<BinaryTreeNode<String>>>> = LinkedList::new();
    let mut iterated_queue: LinkedList<String> = LinkedList::new();
    nodes_stack.push_back(Box::new(tree.root_node.clone()));
    iterated_queue.push_back(tree.root_node.clone().unwrap().value);
    while !nodes_stack.is_empty() {
        let mut temp_nodes_stack = LinkedList::new();
        for item in nodes_stack.iter() {
            let left = item.clone().unwrap().left_node;
            let right = item.clone().unwrap().right_node;
            if left.is_some() {
                temp_nodes_stack.push_back(left.clone());
                iterated_queue.push_back(left.clone().unwrap().value);
            }
            if right.is_some() {
                temp_nodes_stack.push_back(right.clone());
                iterated_queue.push_back(right.clone().unwrap().value);
            }
        }
        nodes_stack = temp_nodes_stack;
    }
    return iterated_queue;
}

// 3. 前序遍历最后一个节点
fn last_node_in_preorder_iteration(tree: &BinaryTree<String>) -> String {
    let mut nodes_stack: LinkedList<Box<Option<BinaryTreeNode<String>>>> = LinkedList::new();
    let mut last_element: String;
    nodes_stack.push_back(Box::new(tree.root_node.clone()));
    last_element = tree.root_node.clone().unwrap().value;
    while !nodes_stack.is_empty() {
        if nodes_stack.back().unwrap().is_some() {
            let node = nodes_stack.back().unwrap().clone().unwrap().left_node;
            nodes_stack.push_back(node.clone());
            if node.is_some() {
                last_element = node.unwrap().value;
            }
        } else {
            nodes_stack.pop_back();
            if !nodes_stack.is_empty() {
                let node = nodes_stack.pop_back().unwrap().unwrap().right_node;
                nodes_stack.push_back(node.clone());
                if node.is_some() {
                    last_element = node.unwrap().value;
                }
            }
        }
    }
    return last_element;
}

// 3. 后序遍历最后一个节点
fn first_node_in_lastorder_iteration(tree: &BinaryTree<String>) -> String {
    let mut nodes_stack: LinkedList<Box<Option<BinaryTreeNode<String>>>> = LinkedList::new();
    let mut last_element: String = "".to_string();
    nodes_stack.push_back(Box::new(tree.root_node.clone()));
    while !nodes_stack.is_empty() {
        if nodes_stack.back().unwrap().is_some() {
            let node = nodes_stack.back().unwrap().clone().unwrap().left_node;
            nodes_stack.push_back(node.clone());
            if node.is_none() {
                nodes_stack.pop_back();
                last_element = nodes_stack.back().unwrap().clone().unwrap().value;
                return last_element;
            }
        }
    }
    // never reach there!
    return last_element;
}

// 5. 左右子女互换
fn exchange_children(tree: &mut BinaryTree<String>) -> LinkedList<String> {
    let mut nodes_stack: LinkedList<Box<Option<BinaryTreeNode<String>>>> = LinkedList::new();
    let mut iterated_queue: LinkedList<String> = LinkedList::new();
    nodes_stack.push_back(Box::new(tree.root_node.clone()));
    iterated_queue.push_back(tree.root_node.clone().unwrap().value);
    while !nodes_stack.is_empty() {
        let mut temp_nodes_stack = LinkedList::new();
        for item in nodes_stack.iter_mut() {
            let mut left = item.clone().unwrap().left_node;
            let mut right = item.clone().unwrap().right_node;

            let temp = left.clone();
            left = right.clone();
            right = temp.clone();
            if left.is_some() {
                temp_nodes_stack.push_back(left.clone());
                //item.as_mut().unwrap().set_left_node(Option::from(left.clone().unwrap()));
                iterated_queue.push_back(left.clone().unwrap().value);
            }
            if right.is_some() {
                temp_nodes_stack.push_back(right.clone());
                //item.as_mut().unwrap().set_right_node(Option::from(right.clone().unwrap()));
                iterated_queue.push_back(right.clone().unwrap().value);
            }
        }
        nodes_stack = temp_nodes_stack;
    }
    return iterated_queue;
}

fn main() {
    murphy::mur_profiler_module("Lab7");
    let expr = "A(B(S,D),C(E,F))";
    println!("示例表达式: {}", expr);
    let mut binary_tree = BinaryTree::<String>::new_from_expr(expr);
    murphy::mur_profiler_test_lambda("1.前序非递归遍历", true, &|| {
        for item in iterate_preorder_nonrecursive(&binary_tree).iter() {
            print!("{} ", item);
        }
        println!();
    });
    murphy::mur_profiler_test_lambda("2.层次非递归遍历", true, &|| {
        for item in iterate_levelorder_non_recursive(&binary_tree).iter() {
            print!("{} ", item);
        }
        println!();
    });
    murphy::mur_profiler_test_lambda("3.前序遍历最后一个节点", true, &|| {
        print!("{}", last_node_in_preorder_iteration(&binary_tree));
        println!();
    });
    murphy::mur_profiler_test_lambda("4.后序遍历第一个节点", true, &|| {
        print!("{}", first_node_in_lastorder_iteration(&binary_tree));
        println!();
    });
    murphy::mur_profiler_test_lambda("5.左右子女互换", true, &|| {
        let mut modified_tree = binary_tree.clone();
        for item in exchange_children(&mut modified_tree).iter() {
            print!("{} ", item);
        }
        println!();
    });
    murphy::mur_profiler_test_lambda("6.表达式建树", true, &|| {
        println!("Expression:{} ", expr);
        print!("层次非递归遍历: ");
        for item in iterate_levelorder_non_recursive(&binary_tree).iter() {
            print!("{} ", item);
        }
        println!("");
    });
}
