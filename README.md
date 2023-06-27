# Blockchain Network: Creating a Decentralized Network of BlockChains

## Objective
The goal of this project is to create a network of BlockChains using the given class structures. This is an extension of Project 3.

## Description
In this project, we have an additional class called `blockNetwork` that includes four fields: `numNodes`, `allNodes`, `u`, and `v`. The input file provides a list of `u-v` values representing the connections between nodes.

## Input File Explanation
The input file is structured as follows:
- Number of Nodes: Specifies the total number of nodes in the network.
- Number of Transactions per Block: Indicates the number of transactions per block.
- Total Number of Transactions: Represents the overall number of transactions.
- Number of Edges: Specifies the number of edges connecting nodes.
- Edge List: Lists the edges connecting nodes `u` and `v`.
- Transactions: Provides the transactions in the format: Transaction ID, FromID, ToID, Amount, and Timestamp.

## Implementation Guidelines
1. Understand the provided class structures and relationships.
2. Parse the input file and extract relevant information.
3. Use the provided boilerplate code as a starting point.
4. Implement logic to track `fromValue` and `toValue` for each transaction.
5. Update the blockchain for each node based on the transactions.

## Running the Program
### Visual Studio (Windows)
1. Open or create a new project.
2. Go to Project > Project Properties.
3. Expand Configuration Properties and select Debugging.
4. In the Command Arguments field, type `<input_filename`.
5. Build and run the program.

### macOS/Linux/PowerShell
If you have g++ installed, compile and run the program using the terminal:

        g++ project4.cpp -o p4
        ./p4 < input1.txt

For powershell:

        Get-Content input1.txt | ./p4

## Constraints
1. Use only the provided header files.
2. This project is not a group project. Consultation and plagiarism are strictly prohibited.

Please review the academic integrity policies in the course modules.

