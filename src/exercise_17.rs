use crate::misc;

static mut REG_A: usize = 0;
static mut REG_B: usize = 0;
static mut REG_C: usize = 0;
static mut IP: usize = 0;
static mut OUTPUT: String = String::new();

fn write_output(value: usize) {
    unsafe {
        OUTPUT += &value.to_string();
        OUTPUT += ",";
    }
}

fn read_output() -> String {
    unsafe {
        let ptr = &raw const OUTPUT;
        let str = (*ptr).as_str();
        let len = str.len() - 1;
        str[0..len].to_string()
    }
}

fn set_regs(registers: &Vec<usize>) {
    let mut iter = registers.iter();
    write_reg('A', *iter.next().unwrap());
    write_reg('B', *iter.next().unwrap());
    write_reg('C', *iter.next().unwrap());
}

fn read_reg(register: char) -> usize {
    unsafe {
        match register {
            'A' => REG_A,
            'B' => REG_B,
            'C' => REG_C,
            _ => panic!(),
        }
    }
}

fn write_reg(register: char, value: usize) {
    unsafe {
        match register {
            'A' => REG_A = value,
            'B' => REG_B = value,
            'C' => REG_C = value,
            _ => panic!(),
        }
    }
}

fn read_operand(combo_operand: usize) -> usize {
    match combo_operand {
        0 => 0,
        1 => 1,
        2 => 2,
        3 => 3,
        4 => read_reg('A'),
        5 => read_reg('B'),
        6 => read_reg('C'),
        _ => panic!(),
    }
}

fn op0_adv(combo_operand: usize) {
    let reg_a = read_reg('A');
    let operand = read_operand(combo_operand) as u32;
    let result = reg_a / usize::pow(2, operand);
    write_reg('A', result);
}

fn op1_bxl(operand: usize) {
    let reg_b = read_reg('B');
    let result = reg_b ^ operand;
    write_reg('B', result);
}

fn op2_bst(combo_operand: usize) {
    let operand = read_operand(combo_operand);
    let result = operand % 8;
    write_reg('B', result);
}

fn op3_jnz(operand: usize) {
    let reg_a = read_reg('A');
    if reg_a != 0 {
        unsafe {
            IP = operand;
        }
    }
}

fn op4_bxc(_: usize) {
    let reg_b = read_reg('B');
    let reg_c = read_reg('C');
    let result = reg_b ^ reg_c;
    write_reg('B', result);
}

fn op5_out(combo_operand: usize) {
    let operand = read_operand(combo_operand);
    let result = operand % 8;
    write_output(result);
}

fn op6_bdv(combo_operand: usize) {
    let reg_a = read_reg('A');
    let operand = read_operand(combo_operand) as u32;
    let result = reg_a / usize::pow(2, operand);
    write_reg('B', result);
}

fn op7_cdv(combo_operand: usize) {
    let reg_a = read_reg('A');
    let operand = read_operand(combo_operand) as u32;
    let result = reg_a / usize::pow(2, operand);
    write_reg('C', result);
}

fn parse_machine(path: &str) -> (Vec<usize>, Vec<usize>) {
    let text = misc::text(path);
    let (registers, instructions) = text.split_once("\n\n").unwrap();
    let registers = registers
        .split("\n")
        .map(|register| register.split_at(12).1.parse().unwrap())
        .collect();
    let instructions = instructions
        .split_at(9)
        .1
        .split(",")
        .map(|value| value.parse().unwrap())
        .collect();

    (registers, instructions)
}

fn perform_instruction(opcode: usize, operand: usize) {
    match opcode {
        0 => op0_adv(operand),
        1 => op1_bxl(operand),
        2 => op2_bst(operand),
        3 => op3_jnz(operand),
        4 => op4_bxc(operand),
        5 => op5_out(operand),
        6 => op6_bdv(operand),
        7 => op7_cdv(operand),
        _ => panic!(),
    }
}

fn run_machine(instructions: &Vec<usize>) {
    unsafe {
        while IP < instructions.len() {
            let (opcode, operand) = (
                *instructions.get(IP).unwrap(),
                *instructions.get(IP + 1).unwrap(),
            );
            let old_ip = IP;
            perform_instruction(opcode, operand);
            if old_ip == IP {
                IP += 2;
            }
        }
    }
}

pub fn a(path: &str) -> String {
    let (registers, instructions) = parse_machine(path);
    set_regs(&registers);
    run_machine(&instructions);
    read_output()
}

pub fn b(_path: &str) -> String {
    "".to_string()
}

#[test]
fn test() {
    assert!(a("input/exercise_17.txt") == "7,3,5,7,5,7,4,3,0");
    // assert!(b("input/exercise_17.txt") == "");
}
