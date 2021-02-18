mkdir -p ./tests/outputs

bash ../compile.sh

echo "> Generating outputs:"

for FILE in 'complex_example_1' 'conditional' 'errors'  'expressions' 'function' 'initialization' 'loops' 'recursion' 'simple' 'tokens'; do
	./yapl "./tests/examples/${FILE}.yapl" > "./tests/outputs/${FILE}.txt"
done

echo ""
echo "> Check the ./tests/outputs/ folder for the test cases outputs"