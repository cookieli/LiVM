class Test1 {
	public static void main(String[] args) {
		StringBuilder sb = new StringBuilder();
		String s = new String("my jvm!");
		sb.append("Hello, " + s);
		System.out.println(sb.toString());
	}
}